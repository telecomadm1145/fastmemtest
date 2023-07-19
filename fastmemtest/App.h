#pragma once
namespace nt
{
	class App
	{
	public:
		static void Create(App*& rout);
		virtual void Startup() = 0;
	};
}