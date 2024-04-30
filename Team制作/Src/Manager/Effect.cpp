#include<EffekseerForDxLib.h>
#include "Effect.h"

Effect::Effect(void)
{
	resType_ = TYPE::NONE;
	path_ = "";

	handleId_ = -1;
}

Effect::Effect(TYPE type, const std::string& path)
{
	resType_ = type;
	path_ = path;
}


Effect::~Effect(void)
{
}

void Effect::Load(void)
{
	switch (resType_)
	{
	case Effect::TYPE::EFKEFC:
		// �摜
		//handleId_ = LoadGraph(path_.c_str());
		handleId_ = LoadEffekseerEffect(path_.c_str());
		playId_ = -1;//�G�t�F�N�g�̃v���CID
		break;

	case Effect::TYPE::PNG:
		
		break;
	}

}

void Effect::Release(void)
{

	switch (resType_)
	{
	case Effect::TYPE::EFKEFC:
		// �摜
		//handleId_ = LoadGraph(path_.c_str());
		DeleteEffekseerEffect(handleId_);
		break;

	case Effect::TYPE::PNG:

		break;
	}
}

//�Đ�
void Effect::Play()
{
	playId_ = PlayEffekseer3DEffect(handleId_);
}

//��~
void Effect::Stop()
{
	StopEffekseer3DEffect(playId_);
}

//�Đ��������`�F�b�N
//true : �Đ���  false : ��~��
bool Effect::IsCheckPlay()
{
	return IsEffekseer3DEffectPlaying(playId_) == 0 ? true : false;
}

//�G�t�F�N�g�̏����Z�b�g
void Effect::SetPos(VECTOR pos)
{
	SetPosPlayingEffekseer3DEffect(playId_, pos.x, pos.y, pos.z);
}

void Effect::SetRot(VECTOR rot)
{
	SetRotationPlayingEffekseer3DEffect(playId_, rot.x, rot.y, rot.z);

}

void Effect::SetScl(VECTOR scl)
{
	SetScalePlayingEffekseer3DEffect(playId_, scl.x, scl.y, scl.z);
}

void Effect::SetSpeed(float speed)
{
	SetSpeedPlayingEffekseer3DEffect(playId_, speed);
}