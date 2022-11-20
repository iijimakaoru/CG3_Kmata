#include "GameScene.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete spriteBG;
	delete particleMan;
	for (int i = 0; i < 2; i++)
	{
		delete object3d[i];
	}
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->input = input;

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/haikeidesu.jpg");

	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	// 3Dオブジェクト生成
	particleMan = ParticleManager::Create();

	emitter = new Emitter();
	emitter->Init(particleMan);
	for (int i = 0; i < 1; i++)
	{
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		emitter->SpawnParticle(60, pos, vel, acc, 1.0f, 0.0f, { 1,0,1,1 });
	}

	particleMan->Update();

	// 3Dオブジェクト生成
	for (int i = 0; i < 2; i++)
	{
		object3d[i] = Object3d::Create();
		object3d[i]->Update();
	}
}

void GameScene::Update()
{
	R += rSpeed;
	G -= gSpeed;
	B += bSpeed;

	if (R > 1.0f || R < 0)
	{
		rSpeed = -rSpeed;
	}

	if (G > 1.0f || G < 0)
	{
		gSpeed = -gSpeed;
	}

	if (B > 1.0f || B < 0)
	{
		bSpeed = -bSpeed;
	}

	// カメラ移動
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		if (input->PushKey(DIK_W)) { ParticleManager::CameraMoveEyeVector({ 0.0f,+1.0f,0.0f }); }
		else if (input->PushKey(DIK_S)) { ParticleManager::CameraMoveEyeVector({ 0.0f,-1.0f,0.0f }); }
		if (input->PushKey(DIK_D)) { ParticleManager::CameraMoveEyeVector({ +1.0f,0.0f,0.0f }); }
		else if (input->PushKey(DIK_A)) { ParticleManager::CameraMoveEyeVector({ -1.0f,0.0f,0.0f }); }

		if (input->PushKey(DIK_W)) { Object3d::CameraMoveEyeVector({ 0.0f,+1.0f,0.0f }); }
		else if (input->PushKey(DIK_S)) { Object3d::CameraMoveEyeVector({ 0.0f,-1.0f,0.0f }); }
		if (input->PushKey(DIK_D)) { Object3d::CameraMoveEyeVector({ +1.0f,0.0f,0.0f }); }
		else if (input->PushKey(DIK_A)) { Object3d::CameraMoveEyeVector({ -1.0f,0.0f,0.0f }); }
	}


	object3d[0]->isBillboard = true;

	debugText.Print("ModeChange:M", 0, 0, 1);
	if (mode == 0)
	{
		debugText.Print("Mode:Billboard", 0, 20, 1);
	}
	if (mode == 1)
	{
		debugText.Print("Mode:Emitter", 0, 20, 1);
	}
	if (mode == 2)
	{
		debugText.Print("Mode:Particle", 0, 20, 1);
	}

	if (input->TriggerKey(DIK_M))
	{
		mode++;
		if (mode > maxMode)
		{
			mode = 0;
		}
	}

	if (mode == 0)
	{
		// オブジェクト移動
		if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
		{
			// 現在の座標を取得
			XMFLOAT3 position = object3d[0]->GetPosition();

			// 移動後の座標を計算
			if (input->PushKey(DIK_UP)) { position.y += 1.0f; }
			else if (input->PushKey(DIK_DOWN)) { position.y -= 1.0f; }
			if (input->PushKey(DIK_RIGHT)) { position.x += 1.0f; }
			else if (input->PushKey(DIK_LEFT)) { position.x -= 1.0f; }

			// 座標の変更を反映
			object3d[0]->SetPosition(position);
		}
	}
	else if (mode == 1)
	{
		float move = 5.0f;
		for (int i = 0; i < 10; i++)
		{
			XMFLOAT3 pos = { 0,0,0 };

			const float rnd_vel = 0.1f;
			XMFLOAT3 vel{};
			vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

			XMFLOAT3 acc{};
			const float rnd_acc = 0.001f;
			acc.y = -(float)rand() / RAND_MAX * rnd_acc;

			emitter->SpawnParticle(60, pos, vel, acc, 1.0f, 0.0f, { R,G,B,1.0f });
			emitter->SpawnParticle(60, { pos.x + move,pos.y + move,0 }, vel, acc, 1.0f, 0.0f, { R,G,B,1.0f });
			emitter->SpawnParticle(60, { pos.x - move,pos.y + move,0 }, vel, acc, 1.0f, 0.0f, { R,G,B,1.0f });
		}
	}
	else if (mode == 2)
	{
		for (int i = 0; i < 10; i++)
		{
			const float rnd_pos = 10.0f;
			XMFLOAT3 pos{};
			pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
			pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

			const float rnd_vel = 0.1f;
			XMFLOAT3 vel{};
			vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

			XMFLOAT3 acc{};
			const float rnd_acc = 0.001f;
			acc.y = -(float)rand() / RAND_MAX * rnd_acc;

			emitter->SpawnParticle(60, pos, vel, acc, 1.0f, 0.0f,{ R,G,B,1.0f });
		}
	}

	particleMan->Update();

	for (int i = 0; i < 2; i++)
	{
		object3d[i]->Update();
	}
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	Object3d::PreDraw(cmdList);

	if (mode == 0)
	{
		for (int i = 0; i < 2; i++)
		{
			object3d[i]->Draw();
		}
	}

	Object3d::PostDraw();
	// 3Dオブジェクト描画前処理
	ParticleManager::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	particleMan->Draw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	ParticleManager::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
