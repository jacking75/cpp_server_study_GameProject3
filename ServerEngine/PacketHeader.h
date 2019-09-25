#pragma once


namespace ServerEngine
{

#pragma  pack(push)
#pragma  pack(1)

#define CLIENT_CODE 0x88
#define SERVER_CODE 0x99

	struct PacketHeader
	{
		UINT32	 CheckCode;
		UINT32   dwMsgID;
		UINT32   dwSize;
		UINT32   dwPacketNo;	//시퀀스 번호 생성 = wCommandID ^ dwSize + index (패키지 당 자동으로 인덱스 증가); 시퀀스 번호 복원 = pHeader-> dwPacketNo-pHeader-> wCommandID ^ pHeader-> dwSize;
		UINT64   u64TargetID;
		UINT32   dwUserData;	//사본에 있는 클라이언트의 경우 dwUserData는 copyguid이며 논리 서비스의 경우 프로토콜의 일부가 클라이언트 연결 ID로 작동한다.
	};

#pragma  pack(pop)



}