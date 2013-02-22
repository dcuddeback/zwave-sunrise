// vim : set foldmethod=syntax :
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>
#include <cstdlib>
#include <stdint.h>
#include <unistd.h>

#include <openzwave/Options.h>
#include <openzwave/Manager.h>
#include <openzwave/Driver.h>
#include <openzwave/Notification.h>
#include <openzwave/platform/Log.h>

#include "config.h"
#include "range.hpp"

static void sunrise(OpenZWave::Manager *manager, const Config &config)
{
  typedef std::chrono::system_clock clock;
  using std::chrono::seconds;
  using std::chrono::duration_cast;
  using std::this_thread::sleep_until;
  using ext::range;

  const auto duration   = seconds(config.duration());
  const auto levels     = range<unsigned int>(config.min_level(), config.max_level(), true);
  const auto num_steps  = levels.steps();
  const auto time_step  = duration_cast<clock::duration>(duration) / num_steps;
  const auto start_time = clock::now();

  auto next_time = start_time;

  for (auto level : levels) {
    sleep_until(next_time);
    next_time += time_step;

    std::cout << "Setting Level: " << level << std::endl;
    manager->SetNodeLevel(config.home_id(), config.node_id(), level);
  }
}

static void print_config(const Config &config)
{
  std::cout << "Device    : " << config.device()         << std::endl;

  std::cout << std::hex;
  std::cout << "Home ID   : " << config.home_id()        << std::endl;

  std::cout << std::dec;
  std::cout << "Node ID   : " << (int)config.node_id()   << std::endl;
  std::cout << "Min Level : " << (int)config.min_level() << std::endl;
  std::cout << "Max Level : " << (int)config.max_level() << std::endl;
  std::cout << "Duration  : " << config.duration()       << std::endl;
}

int main(int argc, char **argv)
{
  using std::this_thread::sleep_for;

  const auto device_delay = std::chrono::seconds(2);

  try {
    const auto config = Config::process_args(argc, argv);
    print_config(config);

    auto options = OpenZWave::Options::Create("/usr/share/openzwave/config", "", "");

    options->AddOptionBool("ConsoleOutput", false);
    options->AddOptionInt("SaveLogLevel",   OpenZWave::LogLevel_Info);
    options->AddOptionInt("QueueLogLevel",  OpenZWave::LogLevel_Debug);
    options->AddOptionInt("DumpTrigger",    OpenZWave::LogLevel_Error);
    options->Lock();

    auto manager = OpenZWave::Manager::Create();

    manager->AddDriver(config.device());
    sleep_for(device_delay);

    sunrise(manager, config);
    sleep_for(device_delay);

    manager->RemoveDriver(config.device());

    OpenZWave::Manager::Destroy();
    OpenZWave::Options::Destroy();
  }
  catch (const Config::Exception &exp) {
    if (exp.is_error()) {
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
