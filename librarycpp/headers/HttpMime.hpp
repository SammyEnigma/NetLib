#ifndef _HTTP_MIME
#define _HTTP_MIME

namespace CoreLib
{
	typedef enum HttpMime
	{
		Binary = 0,
		Text = 1,
		Image = 2,
		Audio = 3,
		Video = 4,
		RichText = 5,
	}HttpMime;
}
#endif