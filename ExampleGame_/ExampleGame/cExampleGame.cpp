// Include Files
//==============

#include "cExampleGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>

// Brick Positions
//-------------
std::vector<eae6320::Math::sVector> brickPositions;

void FillBrickPositions()
{
	//Create the brick for each position
	eae6320::Math::sVector tempBrickPosition;

	//First row from the top
	tempBrickPosition.x = -3.0f;
	tempBrickPosition.y = 3.5f;
	tempBrickPosition.z = 0.0f;
	brickPositions.push_back(tempBrickPosition);
	tempBrickPosition.x = -1.5f;
	tempBrickPosition.y = 3.5f;
	tempBrickPosition.z = 0.0f;
	brickPositions.push_back(tempBrickPosition);
	tempBrickPosition.x = 0.0f;
	tempBrickPosition.y = 3.5f;
	tempBrickPosition.z = 0.0f;
	brickPositions.push_back(tempBrickPosition);
	tempBrickPosition.x = 1.5f;
	tempBrickPosition.y = 3.5f;
	tempBrickPosition.z = 0.0f;
	brickPositions.push_back(tempBrickPosition);
	tempBrickPosition.x = 3.0f;
	tempBrickPosition.y = 3.5f;
	tempBrickPosition.z = 0.0f;
	brickPositions.push_back(tempBrickPosition);

	//Second row
	tempBrickPosition.x = -3.0f;
	tempBrickPosition.y = 2.75f;
	tempBrickPosition.z = 0.0f;
	brickPositions.push_back(tempBrickPosition);
	tempBrickPosition.x = -1.5f;
	tempBrickPosition.y = 2.75f;
	tempBrickPosition.z = 0.0f;
	brickPositions.push_back(tempBrickPosition);
	tempBrickPosition.x = 0.0f;
	tempBrickPosition.y = 2.75f;
	tempBrickPosition.z = 0.0f;
	brickPositions.push_back(tempBrickPosition);
	tempBrickPosition.x = 1.5f;
	tempBrickPosition.y = 2.75f;
	tempBrickPosition.z = 0.0f;
	brickPositions.push_back(tempBrickPosition);
	tempBrickPosition.x = 3.0f;
	tempBrickPosition.y = 2.75f;
	tempBrickPosition.z = 0.0f;
	brickPositions.push_back(tempBrickPosition);

	//Third row
	tempBrickPosition.x = -3.0f;
	tempBrickPosition.y = 2.0f;
	tempBrickPosition.z = 0.0f;
	brickPositions.push_back(tempBrickPosition);
	tempBrickPosition.x = -1.5f;
	tempBrickPosition.y = 2.0f;
	tempBrickPosition.z = 0.0f;
	brickPositions.push_back(tempBrickPosition);
	tempBrickPosition.x = 0.0f;
	tempBrickPosition.y = 2.0f;
	tempBrickPosition.z = 0.0f;
	brickPositions.push_back(tempBrickPosition);
	tempBrickPosition.x = 1.5f;
	tempBrickPosition.y = 2.0f;
	tempBrickPosition.z = 0.0f;
	brickPositions.push_back(tempBrickPosition);
	tempBrickPosition.x = 3.0f;
	tempBrickPosition.y = 2.0f;
	tempBrickPosition.z = 0.0f;
	brickPositions.push_back(tempBrickPosition);
}


// Inherited Implementation
//=========================

// Run
//----

void eae6320::cExampleGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
	{

		if (!isKeyPressedAlready)
		{
			isKeyPressedAlready = true;

			//std::swap(textures[0], textures[1]);
		}
		
	}
	
	if (!UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
	{
		isKeyPressedAlready = false;
	}
}

void eae6320::cExampleGame::UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	//Flash feedback
	if(feedbackCounter <= maxFeedbackCounterValue)
	{ 
		feedbackCounter += i_elapsedSecondCount_sinceLastUpdate;
	}
	else
	{
		feedbackCounter = maxFeedbackCounterValue;
	}

	if (feedbackCounter >= timeToBlack)
	{
		ScreenFeedbackR = 0.0f;
		ScreenFeedbackG = 0.0f;
		ScreenFeedbackB = 0.0f;
	}

	//End state feedback message
	if (endStateCounter <= maxEndStateCounterValue)
	{
		endStateCounter += i_elapsedSecondCount_sinceLastUpdate;
	}
	else
	{
		endStateCounter = maxEndStateCounterValue;
	}

	if (endStateCounter >= timeToErase)
	{
		endStateMessage = 5;
	}

}

void eae6320::cExampleGame::UpdateSimulationBasedOnInput()
{

	isAKeyPressedForBreakOutPaddleMesh = false;

	//Check if the Paddle is within the screen space
	if (breakOutPaddleMeshRigidBody.position.x > spacePaddleMaxScreenWidth)
	{
		breakOutPaddleMeshRigidBody.position.x = spacePaddleMaxScreenWidth;
	}
	if (breakOutPaddleMeshRigidBody.position.x < -spacePaddleMaxScreenWidth)
	{
		breakOutPaddleMeshRigidBody.position.x = -spacePaddleMaxScreenWidth;
	}


	//Paddle Movement right
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right))
	{
		isAKeyPressedForBreakOutPaddleMesh = true;
		breakOutPaddleMeshRigidBody.velocity.x = spacePaddleSpeed;
	}

	//Paddle Movement left
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left))
	{
		isAKeyPressedForBreakOutPaddleMesh = true;
		breakOutPaddleMeshRigidBody.velocity.x = -spacePaddleSpeed;
	}

	//Check if the Ball hit a Space Brick
	for (size_t i = 0; i < brickPositions.size(); i++)
	{
		if (eae6320::Math::Distance(breakOutBallMeshRigidBody.position, brickPositions[i]) <= 0.7f)
		{
			//The ball is on top
			if (breakOutBallMeshRigidBody.position.y > brickPositions[i].y)
			{
				ballSpeedX = -ballSpeedX;
				ballSpeedY = -ballSpeedY;
				brickPositions.erase(brickPositions.begin() + i);
				ScreenFeedbackR = 0.0f;
				ScreenFeedbackG = 0.5f;
				ScreenFeedbackB = 0.0f;
				feedbackCounter = 0.0f;
				break;
			}
			//The ball is on the right
			if (breakOutBallMeshRigidBody.position.x > brickPositions[i].x)
			{
				ballSpeedX = -ballSpeedX;
				brickPositions.erase(brickPositions.begin() + i);
				ScreenFeedbackR = 0.0f;
				ScreenFeedbackG = 0.5f;
				ScreenFeedbackB = 0.0f;
				feedbackCounter = 0.0f;
				break;
			}
			//The ball is on the left
			if (breakOutBallMeshRigidBody.position.x < brickPositions[i].x)
			{
				ballSpeedX = -ballSpeedX;
				brickPositions.erase(brickPositions.begin() + i);
				ScreenFeedbackR = 0.0f;
				ScreenFeedbackG = 0.5f;
				ScreenFeedbackB = 0.0f;
				feedbackCounter = 0.0f;
				break;
			}
			//The ball is below
			if (breakOutBallMeshRigidBody.position.y < brickPositions[i].y)
			{
				ballSpeedX = -ballSpeedX;
				ballSpeedY = -ballSpeedY;
				brickPositions.erase(brickPositions.begin() + i);
				ScreenFeedbackR = 0.0f;
				ScreenFeedbackG = 0.5f;
				ScreenFeedbackB = 0.0f;
				feedbackCounter = 0.0f;
				break;
			}
		}
	}

	//Check if All the bricks were destroyed
	if (brickPositions.size() == 0)
	{
		ballReleased = false;
		breakOutBallMeshRigidBody.velocity.x = 0.0f;
		breakOutBallMeshRigidBody.velocity.y = 0.0f;
		ballSpeedX = 3.0f;
		ballSpeedY = 3.0f;
		breakOutBallMeshRigidBody.position.y = breakOutPaddleMeshRigidBody.position.y + 0.5f;
		FillBrickPositions();
		endStateMessage = 3;
		endStateCounter = 0.0f;
	}

	//Check if the Ball hit the Space Paddle
	if (eae6320::Math::Distance(breakOutBallMeshRigidBody.position,breakOutPaddleMeshRigidBody.position) <= 0.7f)
	{
		//The ball is on top
		if (breakOutPaddleMeshRigidBody.position.y < breakOutBallMeshRigidBody.position.y)
		{
			ballSpeedX = -ballSpeedX;
			ballSpeedY = -ballSpeedY;
		}
		//The ball is on the right
		if (breakOutPaddleMeshRigidBody.position.x < breakOutBallMeshRigidBody.position.x)
		{
			ballSpeedX = -ballSpeedX;
		}
		//The ball is on the left
		if (breakOutPaddleMeshRigidBody.position.x > breakOutBallMeshRigidBody.position.x)
		{
			ballSpeedX = -ballSpeedX;
		}
	}


	//Reset the ball position if it goes below the screen
	if (breakOutBallMeshRigidBody.position.y < -ballMaxScreenWidth)
	{
		ballReleased = false;
		ScreenFeedbackR = 0.5f;
		ScreenFeedbackG = 0.0f;
		ScreenFeedbackB = 0.0f;
		feedbackCounter = 0.0f;
		breakOutBallMeshRigidBody.velocity.x = 0.0f;
		breakOutBallMeshRigidBody.velocity.y = 0.0f;
		ballSpeedX = 3.0f;
		ballSpeedY = 3.0f;
		endStateMessage = 4;
		endStateCounter = 0.0f;
		breakOutBallMeshRigidBody.position.y = breakOutPaddleMeshRigidBody.position.y + 0.5f;
	}
	
	//Check if the Ball is within the screen space and flip velocity
	if (breakOutBallMeshRigidBody.position.x > ballMaxScreenWidth || breakOutBallMeshRigidBody.position.x < -ballMaxScreenWidth)
	{
		ballSpeedX = -ballSpeedX;
	}
	if (breakOutBallMeshRigidBody.position.y > ballMaxScreenWidth)
	{
		ballSpeedY = -ballSpeedY;
	}
	

	//Ball Release
	if (!ballReleased)
	{
		breakOutBallMeshRigidBody.position.x = breakOutPaddleMeshRigidBody.position.x;
	}
	else
	{
		breakOutBallMeshRigidBody.velocity.x = ballSpeedX;
		breakOutBallMeshRigidBody.velocity.y = ballSpeedY;
	}





	if(!isAKeyPressedForBreakOutPaddleMesh)
	{
		//Stop mesh movement
		breakOutPaddleMeshRigidBody.velocity.x = 0.0f;
		breakOutPaddleMeshRigidBody.velocity.y = 0.0f;
	}
		
	//Press Space Bar to Release the Ball
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Space))
	{
		ballReleased = true;
	}




}

void eae6320::cExampleGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	breakOutPaddleMeshRigidBody.Update(i_elapsedSecondCount_sinceLastUpdate);
	breakOutBallMeshRigidBody.Update(i_elapsedSecondCount_sinceLastUpdate);
	gameCamera->m_cameraRigidBody.Update(i_elapsedSecondCount_sinceLastUpdate);
}



void eae6320::cExampleGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{

	

	breakOutPaddleMeshRigidBody.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate);
	breakOutBallMeshRigidBody.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate);
	gameCamera->m_cameraRigidBody.PredictFuturePosition(i_elapsedSecondCount_sinceLastSimulationUpdate);

	//Submit the camera
	eae6320::Graphics::SubmitCamera(gameCamera);

	//Render the mesh with its effect
	
	//eae6320::Graphics::RenderMeshWithEffectAndTextureAtPosition(meshes[0], effects[2], textures[2],meshRigidBody.position);
	//eae6320::Graphics::RenderMeshWithEffectAndTextureAtPosition(meshes[2], effects[2], textures[4],secondStaticMeshPosition);
	
	//Render the Paddle
	eae6320::Graphics::RenderMeshWithEffectAndTextureAtPosition(std::get<0>(meshes[0]), std::get<1>(meshes[0]), std::get<2>(meshes[0]), breakOutPaddleMeshRigidBody.position);

	//Render the Ball
	eae6320::Graphics::RenderMeshWithEffectAndTextureAtPosition(std::get<0>(meshes[1]), std::get<1>(meshes[1]), std::get<2>(meshes[1]), breakOutBallMeshRigidBody.position);

	////Render the Bricks
	for (size_t i = 0; i < brickPositions.size(); i++)
	{
		eae6320::Graphics::RenderMeshWithEffectAndTextureAtPosition(std::get<0>(meshes[i+2]), std::get<1>(meshes[i + 2]), std::get<2>(meshes[i + 2]), brickPositions[i]);
	}

	//Render the end state message
	eae6320::Graphics::RenderSpriteWithEffectAndTexture(sprites[0], effects[0], textures[endStateMessage]);

	//User specify's the background clear color
	eae6320::Graphics::ClearColor(ScreenFeedbackR, ScreenFeedbackG, ScreenFeedbackB,1.0f);
}




// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cExampleGame::Initialize()
{
	//The Game Camera
	cameraRigidBody.position.x = 0.0f;
	cameraRigidBody.position.y = 0.0f;
	cameraRigidBody.position.z = 10.0f;
	gameCamera = new eae6320::Graphics::Camera(cameraRigidBody,45.0f,1.0f,0.1f,100.0f);

	//The BreakOut paddle
	breakOutPaddleMeshRigidBody.position.x = 0.0f;
	breakOutPaddleMeshRigidBody.position.y = -3.8f;
	breakOutPaddleMeshRigidBody.position.z = 0.0f;

	//The BreakOut ball
	breakOutBallMeshRigidBody.position.x = breakOutPaddleMeshRigidBody.position.x;
	breakOutBallMeshRigidBody.position.y = breakOutPaddleMeshRigidBody.position.y + 0.5f;
	breakOutBallMeshRigidBody.position.z = breakOutPaddleMeshRigidBody.position.z;

	//Effect and sprite pointers
	eae6320::Graphics::Effect * newEffect;
	eae6320::Graphics::Sprite * newSprite;


	//Call to factory function for effects
	auto result = eae6320::Graphics::Effect::Factory(newEffect, "sprite", "sprite2", eae6320::Graphics::RenderStates::AlphaTransparency);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	
	effects.push_back(newEffect);

	//Call to effect for rendering meshes
	result = eae6320::Graphics::Effect::Factory(newEffect, "mesh", "mesh", eae6320::Graphics::RenderStates::DepthBuffering);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	effects.push_back(newEffect);



	//Call to factory function for sprites
	result = eae6320::Graphics::Sprite::Factory(newSprite, 0.0f, 0.0f, 1.0f, 1.0f);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	sprites.push_back(newSprite);

	//Texture creation

	//Paddle
	eae6320::Graphics::cTexture::Handle newTexture;

	result = eae6320::Graphics::cTexture::s_manager.Load("data/Textures/SpacePaddle.png", newTexture);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	textureHandles.push_back(newTexture);
	textures.push_back(eae6320::Graphics::cTexture::s_manager.Get(newTexture));

	//Ball

	result = eae6320::Graphics::cTexture::s_manager.Load("data/Textures/Ball.png", newTexture);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	textureHandles.push_back(newTexture);
	textures.push_back(eae6320::Graphics::cTexture::s_manager.Get(newTexture));

	//Bricks

	result = eae6320::Graphics::cTexture::s_manager.Load("data/Textures/SpaceBrick.png", newTexture);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	textureHandles.push_back(newTexture);
	textures.push_back(eae6320::Graphics::cTexture::s_manager.Get(newTexture));

	//End State Messages
	result = eae6320::Graphics::cTexture::s_manager.Load("data/Textures/youWin.png", newTexture);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	textureHandles.push_back(newTexture);
	textures.push_back(eae6320::Graphics::cTexture::s_manager.Get(newTexture));

	result = eae6320::Graphics::cTexture::s_manager.Load("data/Textures/youLose.png", newTexture);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	textureHandles.push_back(newTexture);
	textures.push_back(eae6320::Graphics::cTexture::s_manager.Get(newTexture));

	result = eae6320::Graphics::cTexture::s_manager.Load("data/Textures/blank.png", newTexture);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	textureHandles.push_back(newTexture);
	textures.push_back(eae6320::Graphics::cTexture::s_manager.Get(newTexture));

	//Mesh creation

	//Paddle
	eae6320::Graphics::Mesh::Handle newMesh;

	result = eae6320::Graphics::Mesh::s_manager.Load("data/Meshes/SpacePaddle.fbx", newMesh);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	meshHandles.push_back(newMesh);
	meshes.push_back(std::make_tuple(eae6320::Graphics::Mesh::s_manager.Get(newMesh), effects[1], textures[0], breakOutPaddleMeshRigidBody.position));

	result = eae6320::Graphics::Mesh::s_manager.Load("data/Meshes/Ball.fbx", newMesh);
	if (!result)
	{
		EAE6320_ASSERT(result);
		return Results::Failure;
	}
	meshHandles.push_back(newMesh);
	meshes.push_back(std::make_tuple(eae6320::Graphics::Mesh::s_manager.Get(newMesh), effects[1], textures[1], breakOutBallMeshRigidBody.position));


	FillBrickPositions();

	for (size_t i = 0; i < brickPositions.size(); i++)
	{
		result = eae6320::Graphics::Mesh::s_manager.Load("data/Meshes/SpaceBrick.fbx", newMesh);
		if (!result)
		{
			EAE6320_ASSERT(result);
			return Results::Failure;
		}
		meshHandles.push_back(newMesh);
		//The Brick meshes
		meshes.push_back(std::make_tuple(eae6320::Graphics::Mesh::s_manager.Get(newMesh), effects[1], textures[2], brickPositions[i]));
	}
	return Results::Success;
}

eae6320::cResult eae6320::cExampleGame::CleanUp()
{
	//Destroy the effects
	for (size_t i = 0; i < effects.size(); i++)
	{
		eae6320::Graphics::Effect::Delete(effects[i]);
	}
	//Destroy the sprites
	for (size_t i = 0; i < sprites.size(); i++)
	{
		eae6320::Graphics::Sprite::Delete(sprites[i]);
	}

	//Destroy the textures
	for (size_t i = 0; i < textures.size(); i++)
	{
		eae6320::Graphics::cTexture::s_manager.Release(textureHandles[i]);
	}

	//Destroy the meshes
	for (size_t i = 0; i < meshes.size(); i++)
	{
		eae6320::Graphics::Mesh::s_manager.Release(meshHandles[i]);
	}

	//Delete the camera
	delete gameCamera;
	gameCamera = nullptr;

	return Results::Success;
}
