#ifndef	_NETWORK_DEF
#define	_NETWORK_DEF

namespace CoreLibrary
{
	typedef enum DescriptorType
	{
		FDClient,
		FDServer
	}DescriptorType;

	typedef enum ServerState
	{
		Running = 0,
		NormalExit = 1,
		CreateFailed = 2,
		BindFailed = 3,
		ListenFailed = 4
	}ServerState;

	typedef enum PacketBehaviour
	{
		FixedLength = 0,
		Delimited = 1,
		Undefined = 2
	}PacketBehaviour;

	class Descriptor
	{
	public:
		virtual DescriptorType descriptorType() = 0;
	};
}
#endif