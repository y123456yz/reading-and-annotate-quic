# QUIC Documentation
This documentation primarily tries to sum up the responsibilities of the different classes.

## Classes
There are various classes that implement different aspects of QUIC and often realize an interface which is defined in another class.

### QuicConnection
The QuicConnection class handles the framing for QUIC clients or servers.
It provides a SendStreamData method to send stream data which is called from QuicSession.
It in turn uses the QuicPacketGenerator to generate QuicFrames.
The QuicConnection also implements the QuicPacketGenerator::DelegateInterface and is assigned to a QuicPacketGenerator as a delegate.
The QuicPacketGenerator will then call the OnSerializedPacket method of QuicConnection.
In the end, the frames are then written to the underling connection in the WritePacketInner using a QuicPacketWriter.

### QuicSession
The QuicSession class is a base class from which a concrete session class has to inherit.
It primarily dispatches incoming data onto the correct QUIC stream.
It also owns the QuicConnection, which is used to send the data over the wire.
Therefore, it represents a QUIC connection, consisting of multiple streams and abstracting the real network connection.
The QUIC streams call the WritevData method to send data.
In turn, the QuicConnection will call the methods of the QuicConnectionVisitorInterface to notify the session of new packets or changes to the connection.

### ReliableQuicStream
The ReliableQuicStream class is the base class for QUIC stream implementations.
It defines the interface a QUIC stream class has to satisfy.
It also implements the base logic of streams such as flow control, sequencing of frames, handeling stream or connection reset or close and buffered data writing.
A full QUIC stream class then only has to implement ProcessRawData and EffectivePriority.

### QuicDataStream
The QuicDataStream implements a QUIC stream that transports a SPDY request.
It expects the headers to be delivered out of band in a dedicated header stream that is managed by the session.
The headers are dispatched to it by calling OnStreamHeaders, OnStreamHeadersPriority and OnStreamHeadersComplete.
On initialization it blocks the QuicStreamSequencer until all headers are recieved.

### QuicStreamSequencer
The QuicStreamSequencer buffers frames until they can be passed up to the next layer.
This includes checking for duplicate frames, ordering frames so data will be in order and checking for error conditions.

### QuicPacketCreator
The QuicPacketCreator deals with creating frames and packets.
It can buffer frames to create larger packets that consist of multiple frames and can also generate FEC packets for frames.
It is used by the QuicPacketGenerator to create packets.

### QuicPacketGenerator
The QuicPacketGenerator class is used by the QuicConnection to generate and send packets.
It uses the QuicPacketCreator to build frames and packets.
When a packet is ready, it calls the OnSerializedPacket method of its delegate.

### QuicFramer
The QuicFramer class parses and builds QUIC packets.
It receives data via the ProcessPacket method and calls the methods of the QuicFrameVisitorInterface to inform the QuicConnection about new packets.

### QuicHeadersStream
The QuicHeadersStream transports SPDY headers out of band for the QuicDataConnetion.

## Interfaces
Some classes take an instance of an interface instead of a concrete class.

### QuicPacketWriter
The PacketWriter interface defines the method which will be called by QuicConnection to send packets.
It also defines some helper methods to find out whether the socket is blocked or not.
These methods have to be implemented in the application using QUIC.

### QuicPacketGenerator::DelegateInterface
The QuicPacketGenerator::DelegateInterface defines the methods the QuicPacketGenerator will call when new packets are available.
It is implemented by QuicConnection.

### QuicFrameVisitorInterface
The QuicFrameVisitorInterface defines the methods the QuicFramer will call when new QUIC packets processed.
It is also implemented by QuicConnection.

### QuicConnectionHelperInterface
The QuicConnectionHelperInterface defines some methods the QuicConnection will use to obtain a clock, get a source for random values or set alarms.

### QuicConnectionVisitorInterface
The QuicConnectionVisitorInterface defines methods that are called by QuicConnection when frames are received or other interesting things happen.
It is implemented by QuicSession.
The OnStreamFrame method of this interface is used to hand stream frames from the connection to the session.
