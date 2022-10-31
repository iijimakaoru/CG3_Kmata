#include "BasicShaderHeader.hlsli"
// �O�p�`�̓��͂���_��o�͂���T���v��
[maxvertexcount(3)]
void main(
	point VSOutput input[1] : SV_POSITION, 
	// �_�X�g���[��
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	element.normal = input[0].normal;
	element.uv = input[0].uv;

	element.svpos = input[0].svpos;
	output.Append(element);

	element.svpos = input[0].svpos + float4(10.0f, 10.0f, 0, 0);
	output.Append(element);

	element.svpos = input[0].svpos + float4(10.0f, 0, 0, 0);
	output.Append(element);
}