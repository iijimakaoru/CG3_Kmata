cbuffer cbuff0 : register(b0)
{
	matrix mat; // �R�c�ϊ��s��
};

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 pos : POSITION; // �V�X�e���p���_���W
};
// �W�I���g���V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct GSOutput
{
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
	float2 uv : TEXCOORD; // uv�l
};