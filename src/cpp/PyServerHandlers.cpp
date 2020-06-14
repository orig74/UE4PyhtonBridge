
//#include "PyServerPrivatePCH.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/Components/WindDirectionalSourceComponent.h"
#include "Runtime/Engine/Classes/Engine/WindDirectionalSource.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectBaseUtility.h"
///

#pragma pack(4)
#define V extern "C" __attribute__((visibility("default"))) 
//extern "C" __attribute__((visibility("default"))){
V void* StrToPtr(const char* str)
{
	void* ptr;
	sscanf(str,"%p",&ptr);
	return ptr;
}

V ULevel* GetCurrentLevel(UWorld* uworld)
{
	return uworld->GetCurrentLevel();
}

/*
int GetNumberOfLevelBluePrints(ULevel* level)
{
	TArray < class UBlueprint * > tarray= level->GetLevelBlueprints();
	return tarray.Num();
}

UBlueprint * GetLevelIthBluePrint(ULevel* level,int index)
{
	TArray < class UBlueprint * > tarray= level->GetLevelBlueprints();
	return tarray[index];
}

int BlueprintGetFriendlyName(UBlueprint *bp,uint8* dstname,int dstsize)
{
	FString ret=bp->GetFriendlyName();
	return ret.ToBlob(ret,dstname,dstsize);
}
*/

V int GetActorCount(UWorld* uworld)
{
	return uworld->GetActorCount();
}

V AActor* FindActorByName(UWorld* uworld,char* name,int verbose)
{
	FString fname(name);
	int i=0;
	for (TActorIterator<AActor> ActorItr(uworld); ActorItr;++ActorItr)
	{
		AActor *actor = *ActorItr;
		if(fname==ActorItr->GetName()) return actor;
		//* msg=ActorItr->GetName().GetCharArray();
		if(verbose) UE_LOG(LogTemp, Warning, TEXT("Actor(%d): %s"),i,*ActorItr->GetName());
		//ClientMessage(ActorItr->GetActorLocation().ToString());
		i++;
	}
	return NULL;
}

V int GetActorsNames(UWorld* uworld,wchar_t* outname,int max_size)
{
	FString fname;
	for (TActorIterator<AActor> ActorItr(uworld); ActorItr;++ActorItr)
	{
		fname+=ActorItr->GetName();
		fname+="\n";
	}
	if(fname.Len()>=max_size) return -1;
	for(int i=0;i<fname.Len();i++) outname[i]=fname[i];
	return fname.Len();
}


V void GetActorLocation(AActor* actor,float* outvec)
{
	FVector loc=actor->GetActorLocation();
	for(int i=0;i<3;i++) outvec[i]=loc[i];
}

V void SetActorLocation(AActor* actor,float* invec)
{
	actor->SetActorLocation(FVector(invec[0],invec[1],invec[2]));
}

V void GetActorRotation(AActor* actor,float* outvec)
{
	FRotator rot=actor->GetActorRotation();
	//outvec[0]=rot.Pitch;outvec[1]=rot.Yaw;outvec[2]=rot.Roll;
	outvec[0]=rot.Roll;outvec[1]=rot.Pitch;outvec[2]=rot.Yaw;
}

V void SetActorRotation(AActor* actor,float* invec)
{
	FRotator rot;
	rot.Roll=invec[0];
	rot.Pitch=invec[1];
	rot.Yaw=invec[2];
	actor->SetActorRotation(rot);
}

V void MoveToCameraActor(AActor* actor,ACameraActor* camera,int PlayerIndex)
{
	APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(actor,PlayerIndex);
	OurPlayerController->SetViewTarget(camera);
}


V void GetViewPortSize(int out_sz[2])
{
        UGameViewportClient* gameViewport = GEngine->GameViewport;
        FViewport* InViewport = gameViewport->Viewport;
	out_sz[0]=InViewport->GetSizeXY().X;
	out_sz[1]=InViewport->GetSizeXY().Y;
}

V int TakeScreenshot(void* out_ptr,int length)
{
	UGameViewportClient* gameViewport = GEngine->GameViewport;
	{
		FViewport* InViewport = gameViewport->Viewport;
		TArray<FColor> Bitmap;
		int sx=InViewport->GetSizeXY().X;
		int sy=InViewport->GetSizeXY().Y;
		FIntRect Rect(0, 0, sx, sy);
		bool bScreenshotSuccessful = GetViewportScreenShot(InViewport, Bitmap, Rect);
		if (bScreenshotSuccessful){
			check((Bitmap.Num()*4)<=length);
			memcpy(out_ptr,reinterpret_cast<void*>(Bitmap.GetData()),Bitmap.Num()*4);
			//UE_LOG(LogTemp, Warning, TEXT("bScreenshotSuccessful=True InViewport=%p Bitmap.Num()=%d"),reinterpret_cast<void*>(InViewport),Bitmap.Num());
			return Bitmap.Num();

		} else {
			UE_LOG(LogTemp, Warning, TEXT("bScreenshotSuccessful=False InViewport=%p"),reinterpret_cast<void*>(InViewport));
		}
	}
	return 0;
}

V void SetWindParams(AWindDirectionalSource* awind,float speed,float strength)
{
	UWindDirectionalSourceComponent* windcomp=awind->GetComponent();
	windcomp->Speed=speed;
	windcomp->Strength=strength;
	//FWindSourceSceneProxy * sceneProxy=windcomp->CreateSceneProxy();
}

V void DeactivateActorComponent(UActorComponent* actor)
{
	actor->Deactivate();
}
V void ActivateActorComponent(UActorComponent* actor,bool reset)
{
	actor->Activate(reset);
	actor->BeginPlay();
}


V void GetSceneCapture2DFrustrum(ASceneCapture2D* actor,float* near,float* far)
{
	//UDrawFrustumComponent *frustum=actor->GetDrawFrustum();
	//*near=frustum->MinDrawDistance;
	//*far=frustum->LDMaxDrawDistance;
}

V int GetTextureSize(int out_sz[2],int index,int verbose)
{
	int cnt=0;
	for ( TObjectIterator<UTextureRenderTarget2D> Itr; Itr ; ++Itr)
	{
		if(cnt==index)
		{
			UTextureRenderTarget2D *TextureRenderTarget = *Itr;
			int sx=TextureRenderTarget->SizeX,sy=TextureRenderTarget->SizeY;
			out_sz[0]=sx;
			out_sz[1]=sy;
			if(verbose) UE_LOG(LogTemp, Warning, TEXT("Found texture!! %d,%d"),sx,sy);
			return sx*sy;
		}
		cnt++;
	}
	return 0;

}


V int GetTextureSize2(UTextureRenderTarget2D *TextureRenderTarget ,int out_sz[2])
{
	int sx=TextureRenderTarget->SizeX,sy=TextureRenderTarget->SizeY;
	out_sz[0]=sx;
	out_sz[1]=sy;
	return sx*sy;
}

V UTextureRenderTarget2D* GetTextureByName(const TCHAR* name,int verbose=0)
{
    if(verbose) UE_LOG(LogTemp, Warning, TEXT("looking for texture ==%s=="), name);
    
	return LoadObject<UTextureRenderTarget2D>(NULL, name, NULL, LOAD_None, NULL);
}

V int GetTextureData(UTextureRenderTarget2D* TextureRenderTarget ,void* out_ptr,int length)
{
	int sx=TextureRenderTarget->SizeX,sy=TextureRenderTarget->SizeY;
	TArray<FColor> SurfData;
	FRenderTarget *renderTarget = TextureRenderTarget->GameThread_GetRenderTargetResource();
	check((sx*sy*4)<=length);
	renderTarget->ReadPixels(SurfData);
	memcpy(out_ptr,reinterpret_cast<void*>(SurfData.GetData()),sx*sy*4);
	return sx*sy*4;
}

/*
 * https://forums.unrealengine.com/development-discussion/c-gameplay-programming/1627098-get-image-from-scenecapture2d
 *
void ASceneCaptureRecorder::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  if (OutputVideoFile.IsEmpty()) return;

  if (RenderTarget == nullptr) {
    RenderTarget = NewObject<UTextureRenderTarget2D>();
    RenderTarget->InitCustomFormat(512, 512, EPixelFormat::PF_B8G8R8A8, true);
    RenderTarget->UpdateResourceImmediate();
  }

  SceneCapture->TextureTarget = RenderTarget;
  SceneCapture->CaptureScene();

  auto RenderTargetResource = RenderTarget->GameThread_GetRenderTargetResource();

  if (RenderTargetResource) {
    TArray<FColor> buffer;
    RenderTargetResource->ReadPixels(buffer);

    cv::Mat wrappedImage(RenderTarget->GetSurfaceHeight(), RenderTarget->GetSurfaceWidth(), CV_8UC4,
                         buffer.GetData());

    std::string OutputFile(TCHAR_TO_UTF8(*OutputVideoFile));
    cv::imwrite(OutputFile, wrappedImage);
  }
}
*/

#if 0
V int GetTextureDataf(UTextureRenderTarget2D* TextureRenderTarget ,void* out_ptr,int length,int verbose)
{
	//TArray<FLinearColor> SurfData;
	//SurfData.Init(FLinearColor(1,1,1,1),sx*sy*4*4);
	FTextureRenderTarget2DResource* renderTarget = static_cast<FTextureRenderTarget2DResource*>(TextureRenderTarget->GameThread_GetRenderTargetResource());
	int sx=TextureRenderTarget->SizeX;
	int sy=TextureRenderTarget->SizeY;
	check((sx*sy*4*2)<=length);
	struct FReadSurfaceContext
	{
			FTextureRenderTarget2DResource* renderTarget;
			void* OutData;
			int length;
	};
	FReadSurfaceContext ReadSurfaceContext = {renderTarget,out_ptr,length};
	//ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
    ENQUEUE_RENDER_COMMAND(
    ReadSurfaceCommand,
    FReadSurfaceContext, Context, ReadSurfaceContext,
    {
		uint32 DestStride=0;
		void* cpuDataPtr = (void*)RHILockTexture2D(
	        Context.renderTarget->GetTextureRHI(),
	        0,
	        RLM_ReadOnly,
	        DestStride,
	        false);
		//UE_LOG(LogTemp, Warning, TEXT("got here %d"),__LINE__);
		memcpy(Context.OutData,cpuDataPtr,Context.length);
		RHIUnlockTexture2D(Context.renderTarget->GetTextureRHI(), 0, false);

    });
	//UE_LOG(LogTemp, Warning, TEXT("got here %d"),__LINE__);
    FlushRenderingCommands();
	return length;
}
#endif 

V int GetTextureDataf(UTextureRenderTarget2D* TextureRenderTarget ,void* out_ptr,int length,int verbose)
{
    int sx=TextureRenderTarget->SizeX,sy=TextureRenderTarget->SizeY;
    TArray<FColor> SurfData;
    FRenderTarget *RenderTarget = TextureRenderTarget->GameThread_GetRenderTargetResource();
    check((sx*sy*4*2)<=length);
    RenderTarget->ReadPixels(SurfData);
    memcpy(out_ptr,reinterpret_cast<void*>(SurfData.GetData()),sx*sy*4*2);
    return sx*sy*4;
}

V int GetTexture(void* out_ptr,int length,int index,int verbose)
{
    int cnt=0;
    for ( TObjectIterator<UTextureRenderTarget2D> Itr; Itr ; ++Itr)
    {
        // Access the subclass instance with the * or -> operators.
        if(cnt==index)
        {
            UTextureRenderTarget2D *TextureRenderTarget = *Itr;
            int sx=TextureRenderTarget->SizeX,sy=TextureRenderTarget->SizeY;
            if(verbose) UE_LOG(LogTemp, Warning, TEXT("Found texture!!"));

			TArray<FColor> SurfData;
			FRenderTarget *RenderTarget = TextureRenderTarget->GameThread_GetRenderTargetResource();
			check((sx*sy*4)<=length);
			RenderTarget->ReadPixels(SurfData);
			memcpy(out_ptr,reinterpret_cast<void*>(SurfData.GetData()),sx*sy*4);

			return sx*sy*4;
		}
		cnt++;
	}
	return 0;

}





//} //extern "C"
