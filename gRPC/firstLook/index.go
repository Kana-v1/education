package firstlook

import (
	"context"
	"fmt"
	gRPCkeyValue "keyvalue/gRPCtemplate"
	"log"
	"net"

	"google.golang.org/grpc"
)

type server struct {
	gRPCkeyValue.UnimplementedKeyValueServer
}

func (s *server) Get(ctx context.Context, r *gRPCkeyValue.GetRequest) (*gRPCkeyValue.GetResponse, error) {
	log.Printf("Received GET key = %v", r.Key)

	return &gRPCkeyValue.GetResponse{Value: "1"}, nil
}

func client(action string) {
	conn, _ := grpc.Dial("localhost:5000", grpc.WithInsecure(), grpc.WithBlock())

	defer conn.Close()

	client := gRPCkeyValue.NewKeyValueClient(conn)

	var key, value string

	switch action {
	case "get":
		client.Get(context.Background(), &gRPCkeyValue.GetRequest{Key: key})
	case "put":
		client.Put(context.Background(), &gRPCkeyValue.PutRequest{Key: key, Value: value})
	}
}

func Main() {
	s := grpc.NewServer()

	gRPCkeyValue.RegisterKeyValueServer(s, &server{})

	listener, _ := net.Listen("tcp", ":5000")

	if err := s.Serve(listener); err != nil {
		fmt.Println(err)
	}
}
