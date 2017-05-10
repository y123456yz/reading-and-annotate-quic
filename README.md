# reading-and-annotate-quic
goquic、libquic、quic_client_server_example源码注释分析



一. goquic下载编译安装过程  
===================================    
1. echo $GOPATH查看环境变量路径，然后进入该目录(根据自己实际情况进入相应目录)  
	[root@XX reading-and-annotate-quic]# echo $GOPATH  
	/root/gopath  

2. cd /root/gopath/src/github.com  
	如果没有想要目录，则创建  

3. 在github.com目录下devsisters目录  
	mkdir devsisters;   
	cd devsisters;  

4. 下载代码  
	git clone https://github.com/y123456yz/reading-and-annotate-quic  
	
5. 拷贝reading-and-annotate-quic目录中的goquic文件夹到/root/redisbackup/src/github.com/devsisters/目录下面  

6. 编译  GOQUIC_BUILD=Release ./build_libs.sh   
   如果编译没问题会有如下信息：  
    d/stateless_rejector.o  
	ar: creating libgoquic.a  
	a - build/adaptor.o  
	a - build/adaptor_client.o  
	a - build/chlo_extractor.o  
	a - build/go_ephemeral_key_source.o  
	a - build/go_proof_verifier.o  
	a - build/go_quic_alarm_factory.o  
	a - build/go_quic_client_packet_writer.o  
	a - build/go_quic_client_session.o  
	a - build/go_quic_connection_helper.o  
	a - build/go_quic_dispatcher.o  
	a - build/go_quic_per_connection_packet_writer.o  
	a - build/go_quic_server_packet_writer.o  
	a - build/go_quic_simple_crypto_server_stream_helper.o  
	a - build/go_quic_simple_dispatcher.o  
	a - build/go_quic_simple_server_session.o  
	a - build/go_quic_simple_server_session_helper.o  
	a - build/go_quic_simple_server_stream.o  
	a - build/go_quic_spdy_client_stream.o  
	a - build/go_quic_time_wait_list_manager.o  
	a - build/go_utils.o  
	a - build/proof_source_goquic.o  
	a - build/stateless_rejector.o  
	lib/linux_amd64/ updated  
	
7. 测试   
   [root@centlhw1 goquic]# go build example/server.go   
   [root@centlhw1 goquic]# ./server  
   [root@centlhw1 goquic]# ./server -h  
  Usage of ./server:  
	  -addr string  
			TCP/UDP listen address (default "0.0.0.0")  
	  -cert string  
			Certificate file (PEM), will use encrypted QUIC and SSL when provided  
	  -key string  
			Private key file (PEM), will use encrypted QUIC and SSL when provided   

			
二. 借助libquic库实现的客户端和服务端例子编译方法       
===================================     
1. 先进入libquic-client-server-example/libquic目录，编译出libquic库      
2. 进入libquic-client-server-example/quic_client_server_example目录，编译出quic client和quic server可执行文件      

三. 说明    
===================================     
goquic和libquic-client-server-example都依赖libquic，他们各自一份libquic的原因是: quic_client_server_example参考quic_toy源码, 它是基于libquic老版本开发的，goquic是基于libquic新版本开发。    

编译出现问题参考这里   
===================================  	   
https://github.com/y123456yz/reading-and-annotate-quic/tree/master/libquic%20centos6.5%E7%BC%96%E8%AF%91%E7%8E%AF%E5%A2%83%E8%AE%BE%E7%BD%AE  



文档参考  
===================================  
https://github.com/y123456yz/reading-and-annotate-quic/tree/master/libquic%20centos6.5%E7%BC%96%E8%AF%91%E7%8E%AF%E5%A2%83%E8%AE%BE%E7%BD%AE/%E6%96%87%E6%A1%A3%E8%B5%84%E6%96%99 



nginx http2 quic学习参考:    
===================================    
nginx源码中文详细分析注释参考：https://github.com/y123456yz/reading-code-of-nginx-1.9.2   
libquic goquic编译安装，源码分析注释：https://github.com/y123456yz/reading-and-annotate-quic        
nghttp2相关参考：https://github.com/y123456yz/reading-and-annotate-nghttp2     
		
	