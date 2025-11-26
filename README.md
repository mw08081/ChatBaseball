## 멀티플레이 RPC 통신 찍먹: 숫자 야구
### 주요 구현부
- 각 플레이어별 턴 타이머 생성(도전과제)
  ```c++
  void ACBPlayerState::IncreaseTryCnt()
  {
  	CurTryCnt++;
  	UpdateGameStateString();    // 각 플레이어 위젯에서 시도횟수 출력: 플레이어컨트롤러의 `FString GameStateString(Replicated)`을 업데이트해서 사용
  
      ClearTurnTimer();           // 플레이어의 턴 타이머를 초기화
  	if (CurTryCnt < MaxTryCnt)  // 시도횟수가 남아있다면
  	{
  		StartTimer();             // 턴 타이머 시작
  	}
  }

  void ACBPlayerState::StartTimer()
  {
  	if (GetWorld()->GetTimerManager().IsTimerActive(TurnTimerHandle) == true)
  	{
  		RemainTime--;        // 플레이어 컨트롤러의 GameStateBoardWidet에서 1초마다 갱신하여 표시
  
  		if (RemainTime < 0)
  		{
  			IncreaseTryCnt();  // 타이머 초과시, 시도횟수 1회 증가(입력한 것으로 간주)
  
  			// 타이머 초과로 인해 IncreaseTryCnt()가 호출된다면, JudgeGame()을 해줘야함(특히 마지막 시도일경우)
  			AGameModeBase* GameModeBase = GetWorld()->GetAuthGameMode<AGameModeBase>();
  			if (IsValid(GameModeBase))
  			{
  				ACBGameModeBase* CBGameModeBase = Cast<ACBGameModeBase>(GameModeBase);
  				if (IsValid(CBGameModeBase))
  				{
  					CBGameModeBase->JudgeGame(Cast<ACBPlayerController>(GetOwner()), "OUT");
  				}
  			}
  		}
  	}
  	else
  	{
  		RemainTime = MaxTime;
  		GetWorld()->GetTimerManager().SetTimer(TurnTimerHandle, this, &ThisClass::StartTimer, 1, true);
  	}
  }
  ```

         
- 게임 종료 조건 판별부
  ```c++
  void ACBGameModeBase::JudgeGame(ACBPlayerController* InChatController, const FString& Result)
  {
  	ACBPlayerState* CBPlayerState = InChatController->GetPlayerState<ACBPlayerState>();
  	if (IsValid(CBPlayerState) == false)
  	{
  		return;
  	}
  
  	if (Result.LeftChop(2) == "3S")
  	{
  		InChatController->GameStateString = "You correct the answer!!!";
  		ResetGame(CBPlayerState->PlayerName + TEXT(" has won the game!!!"));        // <- 승자가 있다면 바로 종료
  	}
  	else
  	{
  		int32 TryCnt = CBPlayerState->GetCurTryCnt();
  		int32 MaxCnt = CBPlayerState->GetMaxTryCnt();
  
  		if (TryCnt >= MaxCnt)
  		{
  		   InChatController->GameStateString = "You can't try anymore...Wait another player";
  		   CBPlayerState->ClearTurnTimer();

  		   FinishedPlayerCount++;
         if (FinishedPlayerCount == AllPlayerControllers.Num())                  // <- 게임이 종료된 플레이어의 숫자가 연결된 모든 플레이어의 숫자와 같아진다면, 무승부로 종료
  		   {                                                                    
  		      ResetGame("Draw");
  		   }
  		}
  	}
  }
  ```
- GameStateBoardWidget을 이용한 게임상태 갱신
  ```c++
  class CHATBASEBALL_API UGameStateBoard : public UUserWidget
  {
  	GENERATED_BODY()
  
  public:
  	virtual void NativeConstruct() override;
  	virtual void NativeDestruct() override;
  
  public:
  	UPROPERTY(meta = (BindWidget))
  	TObjectPtr<UTextBlock> TextBox_Timer;
  
  	UPROPERTY(meta = (BindWidget))
  	TObjectPtr<UTextBlock> TextBox_GameState;
  	
  public:
  	void SetTextBox_Timer(const FString& Value);
  	void SetTextBox_GameState(const FString& GameState);
  };
  ```
