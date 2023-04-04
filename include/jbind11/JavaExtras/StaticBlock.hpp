#pragma once

#include <string>

namespace jbind11
{
    class StaticBlock
    {
        private:
            std::string blockCode = "";

        public:
            StaticBlock()
            {

            }

            StaticBlock(std::string blockCode) : blockCode(blockCode)
            {

            }

            const std::string& getBlockCode() const
            {
                return blockCode;
            }
    };
}