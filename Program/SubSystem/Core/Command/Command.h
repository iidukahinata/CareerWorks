/**
* @file    Command.h
* @brief
*
* @date	   2022/09/14 2022îNìxèâî≈
*/


class ICommand
{
public:

	~ICommand() {}
	virtual void Execte() = 0;
	virtual void Undo() = 0;
};

//
// EidtorCommand
//

template<class T>
class EditorCommand : public ICommand
{
	typedef std::function<void(T)> Func;
public:

	EditorCommand(Func&& func, const T& next, const T& pre) : m_func(func), m_next(next), m_pre(pre) {}

	void Execte() override { m_func(m_next); }
	void Undo()   override { m_func(m_pre); }

private:

	T m_next;
	T m_pre;

	Func m_func;
};