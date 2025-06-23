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
#include <utility>
#include <cmath>
#include <deque>

namespace sk {

  constexpr int S_ = 64;
  typedef std::bitset<S_> Frame;
  typedef std::vector<Frame> Stream;
  typedef std::vector<Stream> Streams;
  typedef EcalDataFrame_Ph2 Digi;

  /*! \class  sk::Demonstrator
   *  \brief  Class to test sk algo on BCP board
   *  \author Thomas Schuh
   *  \date   2025, Apr
   */
  class Demonstrator : public edm::one::EDAnalyzer<> {
  public:
    Demonstrator(const edm::ParameterSet& iConfig);
    // analyze single event
    void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) override;

  private:
    // struct to represent a peak
    struct Peak {
      Peak() {}
      Peak(int frame, const std::vector<int>& adc) : frame_(frame), front_(adc[frame - 1]), middle_(adc[frame]), back_(adc[frame + 1]) {
        valid_ = middle_ > front_ && middle_ >= back_;
      }
      // return true is peak structure above threshold found
      bool valid(int threshold) const { return valid_ && middle_ > threshold; }
      // reuces accuracy of adc counts if those are large
      void reduce(int width) {
        const int w = std::ceil(std::log2(middle_));
        if (w < width)
          return;
        const int div = pow(2, w - width);
        front_ /= div;
        middle_ /= div;
        back_ /= div;
      }
      // returns ld flag for given threshold and weights
      bool spike(double threshold, const std::vector<double>& weights) const {
        const int LUT0 = std::floor((middle_ + .5) * (threshold + weights[0]));
        const int LUT1 = std::floor((back_ + .5) * weights[1]);
        const int LUT2 = std::floor((back_ + .5) * (back_ + .5) * weights[2]);
        const double dsp = (middle_ + .5) * ((LUT1 + .5) - (front_ + .5) + (LUT0 + .5)) + (LUT2 + .5);
        return dsp < 0.;
      }
      bool valid_ = false;
      int frame_ = 0;
      int front_ = 0;
      int middle_ = 0;
      int back_ = 0;
    };
    // analyze event packet
    void analyze() const;
    // find interesting crystals
    void produce(std::vector<std::vector<const std::vector<int>*>>&) const;
    // interfere events
    void produce(const std::vector<std::vector<const std::vector<int>*>>&, std::vector<std::vector<int>>&) const;
    // produce input data
    void produce(const std::vector<std::vector<int>>&, Streams&) const;
    // play input data through emulator
    void emulate(const Streams&, Streams&) const;
    // play input data through modelsim
    void simulate(const Streams&) const;
    // compare emulation with simulation
    bool compare(const Streams&) const;
    // converts streams of bv into stringstream
    void convert(const Streams&, std::stringstream&, const std::vector<int>&) const;
    // creates emp file header
    std::string header(const std::vector<int>& links) const;
    // creates 6 frame gap between packets
    std::string infraGap(int& nFrame, int numLinks) const;
    // creates frame number
    std::string frame(int& nFrame) const;
    // converts bv into hex
    std::string hex(const Frame& frame, bool first) const;
    // input ed data
    edm::EDGetTokenT<EBDigiCollectionPh2> edGetToken_;
    // number of clock tics between pakets
    int infraGap_;
    // packet size in BX
    int numEvents_;
    // number of clock tics per bx
    int numFramesBX_;
    // total number of ADCs
    int numTotal_;
    // number of samples per ADC
    int numCounts_;
    // ADC counter offset
    int pedestalADC_;
    // adc count offest above noise
    int thresholdPeak_;
    //
    double thresholdLD_;
    //
    std::vector<double> weightsLD_;
    // number of samples per bx
    int numSamples_;
    // number of bits used per ADC count
    int widthADC_;
    // number of bits used per SK flag
    int widthSK_;
    // reduced number of dynamic msbs used for ADC counts during calculations
    int widthReduced_;
    // number of ADCs
    int numADCs_;
    // number of ADC counts muxed to one channel
    int muxedADCs_;
    // number of SK flags muxed to one channel
    int muxedSKs_;
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
    // number of samples per packet
    int sizePacket_;
    // number of frames used to transport one muxed object
    int numSlicesADC_;
    // number of frames used to transport one muxed object
    int numSlicesLD_;
    // muxed input object split into two frames sliced at these posititions
    std::vector<int> slicesADCs_;
    // muxed input object split into 5 adc counts at these posititions
    std::vector<int> unSlicedADCs_;
    // muxed output object split into two frames sliced at these posititions
    std::vector<int> slicesSKs_;
    // raw input container
    std::vector<std::vector<std::vector<int>>> input_;
  };

  Demonstrator::Demonstrator(const edm::ParameterSet& iConfig)
      : infraGap_(iConfig.getParameter<int>("InfraGap")),
        numEvents_(iConfig.getParameter<int>("NumEvents")),
        numFramesBX_(iConfig.getParameter<int>("NumFramesBX")),
        numTotal_(iConfig.getParameter<int>("NumTotal")),
        numCounts_(iConfig.getParameter<int>("NumCounts")),
        pedestalADC_(iConfig.getParameter<int>("PedestalADC")),
        thresholdPeak_(iConfig.getParameter<int>("ThresholdPeak")),
        thresholdLD_(iConfig.getParameter<double>("ThresholdLD")),
        weightsLD_(iConfig.getParameter<std::vector<double>>("WeightsLD")),
        numSamples_(iConfig.getParameter<int>("NumSamples")),
        widthADC_(iConfig.getParameter<int>("WidthADC")),
        widthSK_(iConfig.getParameter<int>("WidthSK")),
        widthReduced_(iConfig.getParameter<int>("WidthReduced")),
        numADCs_(iConfig.getParameter<int>("NumADCs")),
        muxedADCs_(iConfig.getParameter<int>("MuxedADCs")),
        muxedSKs_(iConfig.getParameter<int>("MuxedSKs")),
        channelsIn_(numADCs_ / muxedADCs_),
        channelsOut_(numADCs_ / muxedSKs_),
        runTime_(iConfig.getParameter<double>("RunTime")),
        dir_(iConfig.getParameter<std::string>("Dir")),
        dirIn_(dir_ + iConfig.getParameter<std::string>("txtInput")),
        dirSim_(dir_ + iConfig.getParameter<std::string>("txtOutputSim")),
        dirEmu_(dir_ + iConfig.getParameter<std::string>("txtOutputEmu")),
        dirDiff_(dir_ + iConfig.getParameter<std::string>("txtOutputDiff")),
        numFrames_(numEvents_ * numFramesBX_ - infraGap_),
        sizePacket_(numEvents_ * numSamples_),
        input_(numEvents_, std::vector<std::vector<int>>(numTotal_)) {
    for (std::vector<std::vector<int>>& event : input_)
      for (std::vector<int>& adc : event)
        adc.reserve(numCounts_);
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
      borders.push_back(0);
      std::partial_sum(widths.begin(), widths.end(), std::back_inserter(borders));
    };
    to_SlicePositions(slicesADCs, slicesADCs_);
    to_SlicePositions(std::vector(muxedADCs_, widthADC_), unSlicedADCs_);
    to_SlicePositions(slicesSKs, slicesSKs_);
    numSlicesADC_ = slicesADCs.size();
    numSlicesLD_ = slicesSKs.size();
    // book input ed produt
    const edm::InputTag inputTag = iConfig.getParameter<edm::InputTag>("InputTag");
    edGetToken_ = consumes<EBDigiCollectionPh2>(inputTag);
  }

  // analyze single event
  void Demonstrator::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    // collect input data
    const EBDigiCollectionPh2& input = iEvent.get(edGetToken_);
    std::vector<std::vector<int>>& event = input_[iEvent_];
    for (int iADC = 0; iADC < numTotal_; iADC++) {
      const Digi& digi = input[iADC];
      std::vector<int>& adc = event[iADC];
      for (int iSample = 0; iSample < numCounts_; iSample++)
        adc.push_back(digi[iSample].adc());
    }
    if (++iEvent_ % numEvents_ > 0)
      return;
    // process packet
    analyze();
    // clear data container
    iEvent_ = 0;
    for (std::vector<std::vector<int>>& event : input_)
      for (std::vector<int>& adc : event)
        adc.clear();
  }

  // analyze event packet
  void Demonstrator::analyze() const {
    // find interesting crystals
    std::vector<std::vector<const std::vector<int>*>> total(numTotal_,
                                                            std::vector<const std::vector<int>*>(numEvents_));
    produce(total);
    // interfere events
    std::vector<std::vector<int>> slr(numADCs_, std::vector<int>(sizePacket_, pedestalADC_));
    produce(total, slr);
    // convert input data
    Streams input(channelsIn_.size());
    for (Stream& stream : input)
      stream.reserve(numFrames_);
    produce(slr, input);
    // play input data through emulator
    Streams emu(channelsOut_.size());
    for (Stream& stream : emu)
      stream.reserve(numFrames_);
    emulate(input, emu);
    // play input data through modelsim
    simulate(input);
    // compare emulation with simulation
    if (compare(emu))
      throw cms::Exception("runtime") << "Bit error detected.";
  }

  // find interesting crystals
  void Demonstrator::produce(std::vector<std::vector<const std::vector<int>*>>& total) const {
    auto max = [this](int sum, const std::vector<int>* adc) {
      return sum + *std::max_element(adc->begin(), adc->end());
    };
    auto acc = [max](const std::vector<const std::vector<int>*>& events) {
      return std::accumulate(events.begin(), events.end(), 0, max);
    };
    for (int iEvent = 0; iEvent < numEvents_; iEvent++)
      for (int iADC = 0; iADC < numTotal_; iADC++)
        total[iADC][iEvent] = &input_[iEvent][iADC];
    std::sort(total.begin(), total.end(), [acc](const auto& lhs, const auto& rhs) { return acc(lhs) > acc(rhs); });
  }

  // interfere events
  void Demonstrator::produce(const std::vector<std::vector<const std::vector<int>*>>& total,
                             std::vector<std::vector<int>>& slr) const {
    for (int iADC = 0; iADC < numADCs_; iADC++) {
      std::vector<int>& adc = slr[iADC];
      for (int iEvent = 0; iEvent < numEvents_; iEvent++) {
        const int offsetSample = iEvent * numSamples_;
        const std::vector<int>& counts = *total[iADC][iEvent];
        for (int iCount = 0; iCount < numCounts_; iCount++) {
          int iSample = offsetSample + iCount;
          if (iSample >= sizePacket_)
            iSample -= sizePacket_;
          adc[iSample] += counts[iCount] - pedestalADC_;
        }
      }
      for (int& count : adc)
        count = std::max(count, 0);
    }
  }

  // produce input data
  void Demonstrator::produce(const std::vector<std::vector<int>>& slr, Streams& streams) const {
    // loop over input channel
    for (int iChannel = 0; iChannel < static_cast<int>(channelsIn_.size()); iChannel++) {
      const int offsetADC = iChannel * muxedADCs_;
      Stream& stream = streams[iChannel];
      for (int iSample = 0; iSample < sizePacket_; iSample++) {
        // create muxed input object
        std::stringstream mux;
        // loop over ADCs muxed into this channel
        for (int iADC = muxedADCs_ - 1; iADC >= 0; iADC--)
          mux << TTBV(slr[offsetADC + iADC][iSample], widthADC_);
        const std::string str = mux.str();
        // split muxed word over two frames
        for (int iFrame = 0; iFrame < numSlicesADC_; iFrame++) {
          const int pos = slicesADCs_[iFrame];
          const int count = slicesADCs_[iFrame + 1] - pos;
          const TTBV slice(str.substr(pos, count));
          stream.push_back(slice.bs());
        }
      }
    }
  }

  // play input data through emulator
  void Demonstrator::emulate(const Streams& input, Streams& output) const {
    Stream& stream = output[0];
    // prepare input
    std::vector<std::vector<int>> adcs(numADCs_);
    for (std::vector<int>& adc : adcs)
      adc.reserve(sizePacket_ + 1);
    for (int iSample = 0; iSample < sizePacket_; iSample++) {
      const int offsetFrame = iSample * numSlicesADC_;
      for (int iChannel = 0; iChannel < static_cast<int>(channelsIn_.size()); iChannel++) {
        const int offsetADC = iChannel * muxedADCs_;
        const Stream& stream = input[iChannel];
        std::stringstream mux;
        for (int iSlice = 0; iSlice < numSlicesADC_; iSlice++) {
          const std::string s = stream[offsetFrame + iSlice].to_string();
          const int pos = S_ - slicesADCs_[1];
          const int count = slicesADCs_[1] - slicesADCs_[0];
          mux << s.substr(pos, count);
        }
        const std::string str = mux.str();
        for (int iADC = 0; iADC < muxedADCs_; iADC++) {
          const int w = widthADC_ * muxedADCs_;
          const int pos = w - unSlicedADCs_[iADC + 1];
          const int count = unSlicedADCs_[iADC + 1] - unSlicedADCs_[iADC];
          const TTBV ttBV(str.substr(pos, count));
          adcs[offsetADC + iADC].emplace_back(std::max(ttBV.val() - pedestalADC_, 0));
        }
      }
    }
    for (std::vector<int>& adc : adcs)
      adc.push_back(adc.front());
    // emulate adc by adc
    std::vector<TTBV> lds(numADCs_, TTBV(0, sizePacket_));
    for (int iADC = 0; iADC < numADCs_; iADC++) {
      const std::vector<int>& adc = adcs[iADC];
      TTBV& ld = lds[iADC];
      for (int iEvent = 0; iEvent < numEvents_; iEvent++) {
        const int offset = iEvent * numSamples_;
        int iSample = (iEvent > 0 ? 0 : 1);
        // find largest peak
        Peak last;
        for (; iSample < numSamples_; iSample++) {
          const Peak peak(offset + iSample, adc);
          if (peak.valid(thresholdPeak_) && (!last.valid_ || peak.middle_ > last.middle_))
            last = peak;
        }
        // reduce adc counts if necessary
        last.reduce(widthReduced_);
        // evaluate and set spike flag
        if (last.valid_ && last.spike(thresholdLD_, weightsLD_))
          ld.set(last.frame_);
      }
    }
    // fill output
    for (int iFrame = 0; iFrame < sizePacket_; iFrame++) {
      std::stringstream mux;
      for (const TTBV& ld : lds)
        mux << ld[iFrame];
      std::string str = mux.str();
      // split 75 ld flags ofer two frames
      for (int iSlice = 0; iSlice < numSlicesLD_; iSlice++) {
        const int pos = slicesSKs_[iSlice];
        const int count = slicesSKs_[iSlice + 1] - pos;
        const TTBV slice(str.substr(pos, count));
        stream.push_back(slice.bs());
      }
    }
  }

  // play input data through modelsim
  void Demonstrator::simulate(const Streams& input) const {
    // convert streams to stringstream
    std::stringstream ss;
    convert(input, ss, channelsIn_);
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
  bool Demonstrator::compare(const Streams& emu) const {
    // convert Streams to stringstream
    std::stringstream ss;
    convert(emu, ss, channelsOut_);
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