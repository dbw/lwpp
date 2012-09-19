#include <lwpp/generic_handler.h>

class HelloWorld : public lwpp::LayoutGenericHandler
{
public:
  int Activate()
  {
    lwpp::LWMessage::Info("Hello World!");
	return 0;
  }
};

lwpp::LayoutGenericAdaptor<HelloWorld> hwInstance("HelloWorld");