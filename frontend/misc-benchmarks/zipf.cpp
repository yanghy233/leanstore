#include "Units.hpp"
#include "leanstore/BTreeAdapter.hpp"
#include "leanstore/Config.hpp"
#include "leanstore/LeanStore.hpp"
#include "leanstore/counters/WorkerCounters.hpp"
#include "leanstore/utils/FVector.hpp"
#include "leanstore/utils/Files.hpp"
#include "leanstore/utils/Parallelize.hpp"
#include "leanstore/utils/RandomGenerator.hpp"
#include "leanstore/utils/ZipfRandom.hpp"
// -------------------------------------------------------------------------------------
#include <gflags/gflags.h>
#include <tbb/tbb.h>

#include "PerfEvent.hpp"
// -------------------------------------------------------------------------------------
#include <iostream>
// -------------------------------------------------------------------------------------
DEFINE_string(in, "", "");
DEFINE_string(out, "", "");
DEFINE_string(op, "convert", "");
DEFINE_uint64(count, 1000, "");
// -------------------------------------------------------------------------------------
using namespace leanstore;
// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
using Key = u64;
using Payload = BytesPayload<128>;
// -------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
  gflags::SetUsageMessage("Leanstore Frontend");
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  // -------------------------------------------------------------------------------------
  tbb::task_scheduler_init taskScheduler(FLAGS_worker_threads);
  // -------------------------------------------------------------------------------------
  chrono::high_resolution_clock::time_point begin, end;
  // -------------------------------------------------------------------------------------
  if (FLAGS_op == "convert") {
    vector<u64> keys;
    utils::fillVectorFromBinaryFile(FLAGS_in.c_str(), keys);
    std::ofstream csv;
    csv.open(FLAGS_out, ios::out | ios::trunc);
    csv << std::setprecision(2) << std::fixed << "i,k" << std::endl;
    for (u64 i = 0; i < keys.size(); i++) {
      csv << i << "," << keys[i] << std::endl;
    }
  } else if (FLAGS_op == "generate") {
    std::ofstream csv;
    csv.open(FLAGS_out, ios::out | ios::trunc);
    csv << std::setprecision(2) << std::fixed << "i,k" << std::endl;
    auto random = std::make_unique<utils::ZipfRandom>(FLAGS_count, FLAGS_zipf_factor);
    for (u64 i = 0; i < FLAGS_count; i++) {
      csv << i << "," << random->rand() % (FLAGS_count) << std::endl;
    }
  }
  // -------------------------------------------------------------------------------------
  return 0;
}
