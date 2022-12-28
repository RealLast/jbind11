#pragma once

#include <vector>
#include <sstream> 
#include "jbind11_throw.hpp"
namespace jbind11
{
    class GenericParams
    {
        private:
            std::vector<uint32_t> indices;
            std::vector<std::string> tags;

            std::string genericIndicesString()
            {
                std::stringstream ss;
                for(size_t i = 0; i < indices.size(); i++)
                {
                    if(i != 0)
                    {
                        ss << ", ";
                    }
                    ss << i;
                }
                return ss.str();
            }

        public:
            GenericParams()
            {

            }

            GenericParams(std::vector<uint32_t> indices) : indices(indices)
            {
                tags = std::vector<std::string>(indices.size(), "T");
            }

            GenericParams(std::vector<uint32_t> indices, std::vector<std::string> tags) : indices(indices), tags(tags)
            {

            }

            bool isParamGeneric(uint32_t index)
            {
                return std::find(this->indices.begin(), this->indices.end(), index) != this->indices.end();
            }

            const std::string& getGenericParamTag(uint32_t index)
            {
                // Technically, index can not be < 0, since it is unsigned integer.
                // But to keep the code clean and safe for future changes, we include both.
                for(size_t i = 0; i < indices.size(); i++)
                {
                    if(indices[i] == index)
                    {
                        return tags[i];
                    }
                }
                JBIND_THROW("Error, cannot get generic param tag for parameter with index " << index << "."
                    << "index has not been registered as generic."
                    << "Generic indices are: " << genericIndicesString());
            
            }


    };
}