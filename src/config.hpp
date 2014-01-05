#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <memory>
#include <string>
#include <exception>
#include <stdexcept>
#include <stdint.h>


class Config
{
public:

  Config
  (
    const std::string &device,
    const uint32_t    home_id,
    const uint8_t     node_id,
    const uint8_t     min_level,
    const uint8_t     max_level,
    const uint32_t    duration
  );

  static Config process_args(int argc, char **argv);

  std::string device(void)    const;
  uint32_t    home_id(void)   const;
  uint8_t     node_id(void)   const;
  uint8_t     min_level(void) const;
  uint8_t     max_level(void) const;
  uint32_t    duration(void)  const;

  void set_device    ( const std::string& device    );
  void set_home_id   ( const uint32_t     home_id   );
  void set_node_id   ( const uint8_t      node_id   );
  void set_min_level ( const uint8_t      min_level );
  void set_max_level ( const uint8_t      max_level );
  void set_duration  ( const uint32_t     duration  );


  class Exception : public std::runtime_error {
  public:
    explicit Exception(const std::string &what = "", bool error = true);
    bool is_error(void) const;

  private:
    bool _error;
  };

private:

  std::string _device;
  uint32_t    _home_id;
  uint8_t     _node_id;
  uint8_t     _min_level;
  uint8_t     _max_level;
  uint32_t    _duration;
};

#endif
