# TBaseballMultiGame
TextBaseball MultiGame

<div align = left>
  
![image](https://github.com/user-attachments/assets/98a4bf47-09a5-484e-9509-f2c5ed0600c8)



<br>

## 목적
- 객체지향과 디자인패턴을 의미있게 적용
- 코드 가독성 및 확장성을 고려하여 작성
- MultiNetWork를 활용한 Chat 멀티게임 구현
- RPC, Replecate를 이용한 BroadCast

<br>

## 기술 스택
[![My Skills](https://skillicons.dev/icons?i=cpp,visualstudio,git,github,unreal,notion&theme=light)](https://skillicons.dev)

<br>

##프로젝트 소개
- Project : Baseball MultiGame Project
- Duration : 2025.03.10 ~ 2025.03.28
- GitHub : https://github.com/JungHyaechang/TBaseballMultiGame
- YouTube : https://www.youtube.com/watch?v=Jttnoeh8Wf8
- 정혜창 / [개발블로그](https://velog.io/@hch9097/posts)

<br>

## Project Part Directory And Main Features <br>

<details>
  <summary> GameMode </summary>

    | -- Source
      | -- GameMode
        | -- BBGameMode  // 게임 시작, 턴, 판정결과, 공지 관리
          | -- BaseballUtility // 난수 생성, 실제 판정 로직
        | -- BBGameState  // 게임 종료시까지 유지되어야하는 데이터 관리
        | -- BBPlayerState // WinCount(Replecate), TryCount(Replecate(Repnotify) 관리
        | -- BBPlayerController // ChatComponent를 통해 UI 화면출력, RPC, Input 관리
          | -- ChatComponent // Message RPC(Server, Client), Key Input Binding 관리 
      | -- UI 
        | -- ChatUIWidget // Widget 통합관리
          | -- ChatBoxWidget // 채팅 입력을 ScrollBox에 더해주는 Widget
            | -- ChatEntryWidget // 채팅 입력을 ScrollBox에 더해주는 Widget
          | -- ChatSystemWidget // 일반 채팅 제외, 공지같은 시스템 채팅을 ScrollBox에 나타나는 Widget
        
### GameMode
 - C++ 라이브러리
   - C++ 라이브러리를 통해 난수 생성, 실제 판정 로직 분리, 캡술화
   - RPC와 Replecate를 활용한 Broadcast
   - Getter, Setter 함수를 적극 사용해 안정성을 높임
 - UI
   - 캡술화를 통해 Widget을 Component화(유지보수성 증가)
</details>

<br>

## 개발 환경 / 개발 도구
- C++ 17 (v14.38)
- MSVC v143
- Unreal Engine 5.5
- Visual Studio 2022
- Git LFS
