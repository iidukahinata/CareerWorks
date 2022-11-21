/**
* @file    ScriptInstance.h
* @brief
*
* @date	   2022/11/20 2022�N�x����
*/
#pragma once


#define BOOST_PYTHON_STATIC_LIB
#include <boost/python/object.hpp>
#include "../IResource.h"

enum ScriptFuncType
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

class Script;

class ScriptInstance : public IResource
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(ScriptInstance)
public:

	/** asset �t�@�C���ƓƎ��f�[�^�̍쐬���s���B*/
	static ScriptInstance* Create(StringView name) noexcept;

	bool Load(StringView path) override;
	void Update() override;

public:

	/** �w��֐����o�^����Ă���ꍇ���s����B*/
	void CallFunction(Script* script, ScriptFuncType type) noexcept;
	void CallTickFunction(Script* script, double deltaTime) noexcept;

	bool HasTickFunction() const noexcept;

private:

	/** �R���p�C�������s�����ꍇ�� null �I�u�W�F�N�g��Ԃ��B*/
	boost::python::object CompileScript(StringView name) noexcept;

	/** �X�N���v�g�̃R���p�C���y�сA�֐��̐ݒ���s���B*/
	void SetUpFunctionList() noexcept;

	/** 
	* ScriptFile ����w��֐��̎擾���s���B 
	* @param name [����] boost python �̎d�l�ɂ�� const char* �Ȃǌ^�̓G���[�������B�G���[��������邽�߂ɂ� char[5] �Ȃǂ̌^�w����s���K�v������B
	*/
	template<typename T>
	bool SetFunction(const T& name, ScriptFuncType type, const boost::python::object& scriptFile) noexcept;

	/** Script �G���[�������o�͂���B*/
	void ErrorHandle() noexcept;

private:

	String m_scriptName;

	boost::python::object m_scriptFile;

	// * �֐��̐ݒ�̗L����ێ�
	Array<bool, ScriptFuncType::Max> m_hasFunctionList;

	// * �֐��I�u�W�F�N�g���X�g
	Array<boost::python::object, ScriptFuncType::Max> m_functionList;
};

template<typename T>
FORCEINLINE bool ScriptInstance::SetFunction(const T& name, ScriptFuncType type, const boost::python::object& scriptFile) noexcept
{
	if (!scriptFile.contains(name))
	{
		return false;
	}

	m_hasFunctionList[type] = true;
	m_functionList[type] = scriptFile[name];

	return true;
}