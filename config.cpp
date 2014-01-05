#include "config.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <memory>
#include <stdint.h>
#include <getopt.h>

#define SECONDS(n) ((n))
#define MINUTES(n) (60u * SECONDS(n))
#define HOURS(n)   (60u * MINUTES(n))

static const char * const DEVICE    = "/dev/ttyUSB0";
static const uint32_t     HOME_ID   = 0x016A0C10u;
static const uint8_t      NODE_ID   = 2u;
static const uint8_t      MIN_LEVEL = 1u;
static const uint8_t      MAX_LEVEL = 99u;
static const unsigned int DURATION  = HOURS(1u);

static const std::string usage_text = "Usage: zwave-sunrise [OPTIONS] HOME NODE";
static const std::string help_text =
  "Arguments:\n"
  "   HOME  The ZWave network hexidecimal Home ID.\n"
  "   NODE  The ZWave dimmer device's Node ID.\n"
  "\n"
  "Options:\n"
  "  -d, --device=DEVICE      The Unix device name for the ZWave controller.\n"
  "                             (Default: /dev/ttyUSB0)\n"
  "  -l, --min-level=LEVEL    The minimum light level. (Default: 1)\n"
  "  -L, --max-level=LEVEL    The maximum light level. (Default: 99)\n"
  "  -D, --duration=INTERVAL  The interval for the sunrise, in number of seconds.\n"
  "                             (Default: 3600)\n"
  "  -h, --help               You're reading it!\n"
  "\n"
  "Example:\n"
  "   The following command initiates the sunrise on device 42 on the ZWave network\n"
  "   with address 0xDEADBEEF, using the serial ZWave controller device at /dev/ttyS0:\n"
  "\n"
  "       zwave-sunrise --device /dev/ttyS0 deadbeef 42";

static uint32_t strtouint32(const char * const s, int base = 10)
{
  uint32_t retval;
  std::istringstream sin(s);

  sin >> std::setbase(base) >> retval;
  return retval;
}

static uint8_t strtouint8(const char * const s, int base = 10)
{
  uint32_t retval;
  std::istringstream sin(s);

  sin >> std::setbase(base) >> retval;
  return retval;
}

Config::Exception::Exception(const std::string &what, bool error) :
  std::runtime_error(what),
  _error(error)
{}

bool Config::Exception::is_error(void) const
{
  return _error;
}

Config::Config
(
  const std::string &device,
  const uint32_t    home_id,
  const uint8_t     node_id,
  const uint8_t     min_level,
  const uint8_t     max_level,
  const uint32_t    duration
) :
  _device(device),
  _home_id(home_id),
  _node_id(node_id),
  _min_level(min_level),
  _max_level(max_level),
  _duration(duration)
{}

std::string Config::device()    const { return _device; }
uint32_t    Config::home_id()   const { return _home_id; }
uint8_t     Config::node_id()   const { return _node_id; }
uint8_t     Config::min_level() const { return _min_level; }
uint8_t     Config::max_level() const { return _max_level; }
uint32_t    Config::duration()  const { return _duration; }

void Config::set_device(const std::string& device)  { _device    = device;    }
void Config::set_home_id(uint32_t home_id)          { _home_id   = home_id;   }
void Config::set_node_id(uint8_t node_id)           { _node_id   = node_id;   }
void Config::set_min_level(uint8_t min_level)       { _min_level = min_level; }
void Config::set_max_level(uint8_t max_level)       { _max_level = max_level; }
void Config::set_duration(uint32_t duration)        { _duration  = duration;  }

Config Config::process_args(int argc, char **argv)
{
  Config config { DEVICE, HOME_ID, NODE_ID, MIN_LEVEL, MAX_LEVEL, DURATION };

  static const char *optstr = "D:l:L:d:h";
  static struct option long_options[] = {
    {"device",    required_argument, 0, 'D'},
    {"min-level", required_argument, 0, 'l'},
    {"max-level", required_argument, 0, 'L'},
    {"duration",  required_argument, 0, 'd'},
    {"help",      no_argument,       0, 'h'},
    {0, 0, 0, 0}
  };

  while (true) {
    int c;
    int option_index = 0;

    c = getopt_long(argc, argv, optstr, long_options, &option_index);

    if (c == -1) { break; }

    switch (c) {
      case 'D':
        config.set_device(optarg);
        break;

      case 'l':
        config.set_min_level(strtouint8(optarg));
        break;

      case 'L':
        config.set_max_level(strtouint8(optarg));
        break;

      case 'd':
        config.set_duration(strtouint32(optarg));
        break;

      case 'h':
        std::cerr << usage_text << std::endl << std::endl;
        std::cerr << help_text << std::endl;
        throw Config::Exception { "help requested", false };
        break;

      case '?':
        throw Config::Exception { "unrecognized option" };
        break;
    }
  }

  if (argc - optind != 2) {
    std::cerr << usage_text << std::endl;
    throw Config::Exception { "missing argument" };
  }

  argv = &argv[optind];

  config.set_home_id(strtouint32(*argv++, 16));
  config.set_node_id(strtouint8(*argv++));

  return config;
}
