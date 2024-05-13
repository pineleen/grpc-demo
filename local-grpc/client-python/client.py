#!/usr/bin/env python
# -*- coding: utf-8 -*-

import uuid

import grpc
import server_pb2
from server_pb2_grpc import LocalInferServiceStub

if __name__ == "__main__":

    channel = grpc.insecure_channel('unix:/tmp/grpc_demo_service.sock')
    stub = LocalInferServiceStub(channel)

    try:
        req = server_pb2.InferRequest()
        request_id = uuid.uuid4()
        req.id = str(request_id)
        req.feature["input"].int_array.int64_elems = [1,2,3]

        response = stub.Infer(req)
        output = response.feature["output"].int_array.int64_elems
        print(output)
    except Exception as e:
        print(f"发生异常, requestid: {request_id} err: {e}")

    print('done')