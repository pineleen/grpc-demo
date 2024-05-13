#include <iostream>
#include <string>
#include <grpc++/grpc++.h>
#include "service.grpc.pb.h"

class LocalInferServiceImp final : public infer::LocalInferService::Service {
    grpc::Status Infer(grpc::ServerContext* context, const infer::PredictRequest* request,
                  infer::PredictResult* response) override {
        auto request_id = request->id();
        auto input = request->feature().at("input");

        std::vector<int64_t> output;
        for (int i = 0; i < input.int_array().int64_elems_size(); i++) {
            output.push_back(input.int_array().int64_elems(i) + 1);
        }

        infer::ExFeature future;
        ::google::protobuf::RepeatedField< int64_t > field{output.begin(), output.end()};
        future.mutable_int_array()->mutable_int64_elems()->CopyFrom(field);
        response->mutable_feature()->insert({"output", future});

        response->set_status(infer::Status::SUCCESS);

        return grpc::Status::OK;
    }
};

void RunServer() {
    // std::string server_address("0.0.0.0:500");
    std::string server_address("unix:/tmp/grpc_demo_service.sock");
    LocalInferServiceImp service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main()
{
    RunServer();

    return 0;
}