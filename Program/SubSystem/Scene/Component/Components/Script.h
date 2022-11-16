/**
* @file    Script.h
* @brief
*
* @date	   2022/11/15 2022�N�x����
*/
#pragma once

#define BOOST_PYTHON_STATIC_LIB
#include <boost/python/object.hpp>
#include "../IComponent.h"

class Script : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Script)
private:

	enum FuncType
	{
		Init,

		Register,

		UnRegister,

		Start,

		Stop,

		Remove,

		Update,

		Max,
	};

public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void OnRegister() override;
	void OnUnRegister() override;
	void OnStart() override;
	void OnStop() override;
	void OnRemove() override;

	void Tick(double deltaTime) override;

public:

	void SetScript(StringView name) noexcept;

private:

	/** �R���p�C�������s�����ꍇ�� null �I�u�W�F�N�g��Ԃ��B*/
	boost::python::object CompileScript(StringView name) noexcept;

	/** �X�N���v�g�̃R���p�C���y�сA�֐��̐ݒ���s���B*/
	void SetUpFunctionList() noexcept;

	/** ScriptFile ����w��֐��̎擾���s���B*/
	bool GetFunction(StringView funcName, FuncType type, const boost::python::object& scriptFile) noexcept;

	/** �w��֐����o�^����Ă���ꍇ���s����B*/
	void CallFunction(FuncType type) noexcept;

	/** Script �G���[�������o�͂���B*/
	void ErrorHandle() noexcept;

private:

	String m_scriptName;

	// * �֐��̐ݒ�̗L����ێ�
	Array<bool, FuncType::Max> m_hasFunctionList;

	// * �֐��I�u�W�F�N�g���X�g
	Array<boost::python::object, FuncType::Max> m_functionList;
};