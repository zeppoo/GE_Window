#pragma once

#include "ve_globals.hpp"
#include "ve_utilities.hpp"

namespace ve
{
    void createGraphicsPipeline(PipelineConfigInfo& configInfo);

    static std::vector<char> readFile(const std::string &filename);

    static VkShaderModule createShaderModule(const std::vector<char> &code);

    static VkPipelineShaderStageCreateInfo createVertShaderStageInfo(VkShaderModule module);

    static VkPipelineShaderStageCreateInfo createFragShaderStageInfo(VkShaderModule module);
}