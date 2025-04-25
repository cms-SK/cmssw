#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/L1TrackTrigger/interface/TTBV.h"

#include <sstream>
#include <bitset>
#include <vector>
#include <string>
#include <fstream>
#include <numeric>
#include <set>
#include <cstdlib>
#include <algorithm>
#include <iterator>

#include <iostream>

namespace sk {

  constexpr int S_ = 64;
  typedef std::bitset<S_> Frame;
  typedef std::vector<Frame> Stream;
  typedef std::vector<Stream> Streams;

  /*! \class  sk::Demonstrator
   *  \brief  Class to test sk algo on BCP board
   *  \author Thomas Schuh
   *  \date   2025, Apr
   */
  class Demonstrator : public edm::one::EDAnalyzer<> {
  public:
    Demonstrator(const edm::ParameterSet& iConfig);
    void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) override;

  private:
    // produce partial input data
    void consume(const EBDigiCollection& ebDigiCollection);
    // play input data through emulator
    void emulate();
    // play input data through modelsim
    void simulate() const;
    // compare emulation with simulation
    bool compare() const;
    // converts streams of bv into stringstream
    void convert(const Streams& streams, std::stringstream& ss, const std::vector<int>& mapping) const;
    // creates emp file header
    std::string header(const std::vector<int>& links) const;
    // creates 6 frame gap between packets
    std::string infraGap(int& nFrame, int numLinks) const;
    // creates frame number
    std::string frame(int& nFrame) const;
    // converts bv into hex
    std::string hex(const Frame& frame, bool first) const;
    // input ed data
    edm::EDGetTokenT<EBDigiCollection> edGetToken_;
    // number of clock tics between pakets
    int infraGap_;
    // packet size in BX
    int packetSize_;
    // number of clock tics per bx
    int numFramesBX_;
    // number of samples per bx
    int numSamples_;
    // number of bits used per ADC count
    int widthADC_;
    // number of bits used per SK flag
    int widthSK_;
    // number of ADCs
    int numADCs_;
    // number of ADC counts muxed to one channel
    int muxedADCs_;
    // number of SK flags muxed to one channel
    int muxedSKs_;
    // number of input channel
    int numChannelsIn_;
    // number of output channel
    int numChannelsOut_;
    // used input channels, leave blank for default (0 to n-1)
    std::vector<int> channelsIn_;
    // used output channels, leave blank for default (0 to n-1)
    std::vector<int> channelsOut_;
    // modelsim simulation time
    double runTime_;
    // path to ipbb project area
    std::string dir_;
    // path to input data txt file
    std::string dirIn_;
    // path to simulated output data txt file
    std::string dirSim_;
    // path to emulated output data txt file
    std::string dirEmu_;
    // path to diff eum vs sim txt file
    std::string dirDiff_;
    // current event counter
    int iEvent_ = 0;
    // num Frames per packet
    int numFrames_;
    // number of frames used to transport one muxed object
    int numSlices_;
    // muxed input object split into two frames sliced at these posititions
    std::vector<int> slicesADCs_;
    // muxed output object split into two frames sliced at these posititions
    std::vector<int> slicesSKs_;
    // input data container
    Streams input_;
    // output data container
    Streams output_;
  };

  Demonstrator::Demonstrator(const edm::ParameterSet& iConfig)
      : infraGap_(iConfig.getParameter<int>("InfraGap")),
        packetSize_(iConfig.getParameter<int>("PacketSize")),
        numFramesBX_(iConfig.getParameter<int>("NumFramesBX")),
        numSamples_(iConfig.getParameter<int>("NumSamples")),
        widthADC_(iConfig.getParameter<int>("WidthADC")),
        widthSK_(iConfig.getParameter<int>("WidthSK")),
        numADCs_(iConfig.getParameter<int>("NumADCs")),
        muxedADCs_(iConfig.getParameter<int>("MuxedADCs")),
        muxedSKs_(iConfig.getParameter<int>("MuxedSKs")),
        numChannelsIn_(numADCs_ / muxedADCs_),
        numChannelsOut_(numADCs_ / muxedSKs_),
        channelsIn_(numChannelsIn_),
        channelsOut_(numChannelsOut_),
        runTime_(iConfig.getParameter<double>("RunTime")),
        dir_(iConfig.getParameter<std::string>("Dir")),
        dirIn_(dir_ + iConfig.getParameter<std::string>("txtInput")),
        dirSim_(dir_ + iConfig.getParameter<std::string>("txtOutputSim")),
        dirEmu_(dir_ + iConfig.getParameter<std::string>("txtOutputEmu")),
        dirDiff_(dir_ + iConfig.getParameter<std::string>("txtOutputDiff")),
        numFrames_(packetSize_ * numFramesBX_ - infraGap_),
        input_(channelsIn_.size(), Stream(numFrames_)),
        output_(channelsOut_.size(), Stream(numFrames_)) {
    // input link mapping
    const std::vector<int>& channelsIn = iConfig.getParameter<std::vector<int>>("ChannelsIn");
    if (channelsIn.empty())
      std::iota(channelsIn_.begin(), channelsIn_.end(), 0);
    else
      channelsIn_ = channelsIn;
    // output link mapping
    const std::vector<int>& channelsOut = iConfig.getParameter<std::vector<int>>("ChannelsOut");
    if (channelsOut.empty())
      std::iota(channelsOut_.begin(), channelsOut_.end(), 0);
    else
      channelsOut_ = channelsOut;
    // prepare slice positions
    const std::vector<int>& slicesADCs = iConfig.getParameter<std::vector<int>>("SlicesADCs");
    const std::vector<int>& slicesSKs = iConfig.getParameter<std::vector<int>>("SlicesSKs");
    auto to_SlicePositions = [](const std::vector<int>& widths, std::vector<int>& borders) {
      borders.reserve(widths.size() + 1);
      borders.push_back(1);
      std::partial_sum(widths.begin(), widths.end(), std::back_inserter(borders));
    };
    to_SlicePositions(slicesADCs, slicesADCs_);
    to_SlicePositions(slicesSKs, slicesSKs_);
    numSlices_ = slicesADCs.size();
    // book input ed produt
    const edm::InputTag inputTag = iConfig.getParameter<edm::InputTag>("InputTag");
    edGetToken_ = consumes<EBDigiCollection>(inputTag);
  }

  void Demonstrator::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    // get input ed product
    const EBDigiCollection& ebDigiCollection = iEvent.get(edGetToken_);
    // produce partial input data
    consume(ebDigiCollection);
    // catch 
    if (++iEvent_ % packetSize_ == 0) {
      // play input data through emulator
      emulate();
      // play input data through modelsim
      simulate();
      // compare emulation with simulation
      if (compare())
        // disagreement
        throw cms::Exception("runtime") << "Bit error detected.";
      // clear data container
      input_.clear();
      output_.clear();
    }
  }

  // produce partial input data
  void Demonstrator::consume(const EBDigiCollection& ebDigiCollection) {
    const int eventOffset = iEvent_ * numSlices_ * numSamples_;
    // loop over samples
    for (int iSample = 0; iSample < numSamples_; iSample++) {
      const int offsetFrame = eventOffset + iSample * numSlices_;
      // loop over input channel
      for (int iChannel = 0; iChannel < numChannelsIn_; iChannel++) {
        Stream& stream = input_[iChannel];
        const int offsetADC = iChannel * muxedADCs_;
        // create muxed input object
        std::stringstream mux;
        // loop over ADCs muxed into this channel
        for (int iADC = 0; iADC < muxedADCs_; iADC++)
          // add ADC count to muxed word
          mux << TTBV(ebDigiCollection[offsetADC + iADC][iSample], widthADC_);
        // split muxed word over two frames
        for (int iFrame = 0; iFrame < numSlices_; iFrame++)
          stream[offsetFrame + iFrame] = TTBV(mux.str().substr(slicesADCs_[iFrame], slicesADCs_[iFrame + 1])).bs();
      }
    }
  }

  // play input data through emulator
  void Demonstrator::emulate() {
    // to be done
  }

  // play input data through modelsim
  void Demonstrator::simulate() const {
    // convert streams to stringstream
    std::stringstream ss;
    convert(input_, ss, channelsIn_);
    // write ss to disk
    std::fstream fs;
    fs.open(dirIn_.c_str(), std::fstream::out);
    fs << ss.rdbuf();
    fs.close();
    // run modelsim
    std::stringstream cmd;
    cmd << "cd " << dir_ << " && ./run_sim -quiet -c work.top -do 'run " << runTime_ << "us' -do 'quit' &> /dev/null";
    std::system(cmd.str().c_str());
  }

  // compare emulation with simulation
  bool Demonstrator::compare() const {
    // convert Streams to stringstream
    std::stringstream ss;
    convert(output_, ss, channelsOut_);
    // write emulation output to disk;
    std::fstream fs;
    fs.open(dirEmu_.c_str(), std::fstream::out);
    fs << ss.rdbuf();
    fs.close();
    // use linux diff on disk
    const std::string cmd = "diff " + dirEmu_ + " " + dirSim_ + " &> " + dirDiff_;
    std::system(cmd.c_str());
    ss.str("");
    ss.clear();
    // read diff output
    fs.open(dirDiff_.c_str(), std::fstream::in);
    ss << fs.rdbuf();
    fs.close();
    // count lines, 4 are expected
    int n(0);
    std::string token;
    while (std::getline(ss, token))
      n++;
    return n != 4;
  }

  // converts streams of bv into stringstream
  void Demonstrator::convert(const Streams& streams, std::stringstream& ss, const std::vector<int>& mapping) const {
    // number of transceiver per quad
    static constexpr int quad = 4;
    // prepare listed links
    std::set<int> quads;
    for (int channel : mapping)
      quads.insert(channel / quad);
    std::vector<int> links;
    links.reserve(quads.size() * quad);
    for (int q : quads) {
      const int offset = q * quad;
      for (int c = 0; c < quad; c++)
        links.push_back(offset + c);
    }
    // start with header
    ss << header(links);
    int nFrame(0);
    // create one packet per SLR
    bool first = true;
    // start with emp 6 frame gap
    ss << infraGap(nFrame, links.size());
    for (int iFrame = 0; iFrame < numFrames_; iFrame++) {
      // write one frame for all channel
      ss << frame(nFrame);
      for (int link : links) {
        const auto channel = std::find(mapping.begin(), mapping.end(), link);
        if (channel == mapping.end())
          ss << "  0000 " << std::string(S_ / 4, '0');
        else {
          const Stream& stream = streams[std::distance(mapping.begin(), channel)];
          ss << hex(stream[iFrame], first);
        }
      }
      ss << std::endl;
      first = false;
    }
  }

  // creates emp file header
  std::string Demonstrator::header(const std::vector<int>& links) const {
    std::stringstream ss;
    // file header
    ss << "Id: CMSSW" << std::endl;
    ss << "Metadata: (strobe,) start of orbit, start of packet, end of packet, valid" << std::endl;
    ss << std::endl;
    // link header
    ss << "      Link  ";
    for (int link : links)
      ss << "            " << std::setfill('0') << std::setw(3) << link << "        ";
    ss << std::endl;
    return ss.str();
  }

  // creates 6 frame gap between packets
  std::string Demonstrator::infraGap(int& nFrame, int numLinks) const {
    std::stringstream ss;
    for (int gap = 0; gap < infraGap_; gap++) {
      ss << frame(nFrame);
      for (int link = 0; link < numLinks; link++)
        ss << "  0000 " << std::string(S_ / 4, '0');
      ss << std::endl;
    }
    return ss.str();
  }

  // creates frame number
  std::string Demonstrator::frame(int& nFrame) const {
    std::stringstream ss;
    ss << "Frame " << std::setfill('0') << std::setw(4) << nFrame++ << "  ";
    return ss.str();
  }

  // converts bv into hex
  std::string Demonstrator::hex(const Frame& frame, bool first) const {
    std::stringstream ss;
    ss << (first ? "  1001 " : "  0001 ") << std::setfill('0') << std::setw(S_ / 4) << std::hex << frame.to_ullong();
    return ss.str();
  }

}  // namespace sk

DEFINE_FWK_MODULE(sk::Demonstrator);