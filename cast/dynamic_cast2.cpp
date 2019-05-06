#include <iostream>
#include <string>
using namespace std;
class Window
{
	public:
		Window()
		{
		}
		Window(const string s):name(s) 
	{
	};
		virtual ~Window() 
		{
		};
		void getName() 
		{
			cout << name << endl;
		};
	private:
		string name;
};
class ScrollWindow : public Window
{
	public:
		ScrollWindow(string s) : Window(s) 
	{
	};
		~ScrollWindow() 
		{
		};
		void scroll() 
		{
			cout << "scroll()" << endl;
		};
};
void DoSomething(Window *w)
{
	w->getName();
	 //w->scroll();  // class "Window" has no member scroll
	/* check if the pointer is pointing to a scroll window */
	ScrollWindow *sw = dynamic_cast<ScrollWindow*>(w); // IMPORTANT 
	/* if not null, it's a scroll window object */
	if(sw) 
		sw->scroll();
}
int main()
{
	Window *w = new Window("plain window");
	ScrollWindow *sw = new ScrollWindow("scroll window");
	DoSomething(w);
	DoSomething(sw);

	return 0;
} 
