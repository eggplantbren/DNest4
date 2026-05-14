#pragma once
#include "popen2.hpp"
#include <string>
#include <vector>
#include <unistd.h>
#include "JsonLite.hpp"   // your tiny parser

class ModelServer {
private:
    Popen2 proc;

public:
    ModelServer(const std::string& cmd)
        : proc(cmd)
    {}

    std::string request_raw(const std::string& s) const {
        std::string msg = s + "\n";
        ssize_t n = write(proc.to_child, msg.c_str(), msg.size());
        (void)n; // ignore warning

        char buffer[4096];
        ssize_t m = read(proc.from_child, buffer, sizeof(buffer)-1);
        if (m <= 0)
            throw std::runtime_error("Model server closed pipe");

        buffer[m] = '\0';

        std::string reply(buffer);   // <-- define reply
        return reply;
    }


    int get_num_params() {
        auto reply = request_raw("{\"type\":\"num_params\"}");
        return JsonLite::to_int(JsonLite::extract(reply, "num_params"));
    }

    double log_likelihood(const std::vector<double>& params) {
        std::stringstream req;
        req << std::setprecision(17);

        req << "{\"type\":\"loglik\",\"params\":[";
        for(size_t i = 0; i < params.size(); i++) {
            req << std::to_string(params[i]);
            if(i + 1 < params.size()) req << ",";
        }
        req << "]}";

        auto reply = request_raw(req.str());

        return JsonLite::to_double(JsonLite::extract(reply, "loglik"));
    }

    std::vector<double> prior_transform(const std::vector<double>& us) {

        std::stringstream req;
        req << std::setprecision(17);
        req << "{\"type\":\"prior\",\"us\":[";
        for(size_t i = 0; i < us.size(); i++) {
            req << us[i];
            if(i + 1 < us.size()) req << ",";
        }
        req << "]}";

        auto reply = request_raw(req.str());

        return JsonLite::to_vec(JsonLite::extract(reply, "params"));
    }
};

