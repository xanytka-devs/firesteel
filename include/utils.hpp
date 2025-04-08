#ifndef FSE_UTILS
#define FSE_UTILS

#include "../engine/include/utils/json.hpp"
#include "../engine/include/common.hpp"

const std::string GLOBAL_VER = "0.2.0.6";

glm::vec4 Vec4FromJson(nlohmann::json_abi_v3_11_3::detail::iter_impl<nlohmann::json> tParsable) {
    glm::vec4 a{};
    unsigned int i = 0;
    for (auto v = tParsable->begin(); v != tParsable->end(); ++v) {
        a[i] = v.value();
        i++;
    }
    return a;
}

glm::vec4 Vec4FromJson(nlohmann::json* tParsable) {
    glm::vec4 a{};
    unsigned int i = 0;
    for (auto v = tParsable->begin(); v != tParsable->end(); ++v) {
        a[i] = v.value();
        i++;
    }
    return a;
}

#endif // !FSE_UTILS
