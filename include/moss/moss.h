#pragma once

#include <expected>
#include <span>
#include <string>

#include "moss/types.h"

namespace moss {

/// Runs `rom` until execution enters an infinite loop.
std::expected<void, std::string> Run(std::span<u8> rom);

}