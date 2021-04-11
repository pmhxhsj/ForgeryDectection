# ForgeryDectection

## Digital Image Forgery Detection Using L0_Norm Local Effects

### 프로젝트 개요<br/>
***


한국연구재단에서 지원한 디지털 영상 위변조 탐지를 위한 다차원 영상특징 매트릭스 생성과 통계적 검증 모델 개발연구이다.


### 프로젝트 설명<br/>
***
디지털 이미지 위조 기술은 계속 발전하며, 시간이 갈수록 아무나 간단하게 할 수 있게 되었다. 그러나 위조를 검출하는 방법은
많이 알려지지 않았다. 이러한 문제를 해결하기 위해 L0_Norm을 이용한 복사-붙여넣기 위조 검출 알고리즘을 고안하였다.
복사-붙여넣기 위조가된 비압축 이미지(jpg,png,bmp등)에 대해 L0_Norm을 이용해 위조된 부분의 영역을 검출하는 프로젝트이다.
이미지를 넣고 DCT(이산 코사인 변환 8x8블록에서 진행함)을 진행해 픽셀값을 주파수 값으로 변환한다. 그 후 L0_Norm의
특성을 이용해 모든 주파수값의 값을 정해준다. 그 후 8x8블록에서 Local_minima(최솟값)를 구해 8x8블록에서 최솟값이 
중복되지 않는 값이 있으면 보존하고, 8x8블록에서 최솟값이 중복된다면(1개 이상) 255로 채운다. 이렇게 되면 일치하지 않는
영역을 눈으로 확인할 수 있다. 그러나 더 잘 보이게 하기위해 BAG(Block artifact grid)를 그려 시각적 효과를 증폭시킨다.
그 후 적절한 조건을 걸어 복사-붙여넣기된 영역의 결과값의 정확도를 높혀준다. 기존 연구인 L2_LE를 이용한 연구와 
L1_LE를 이용한 연구보다 모든 압축정도에 대해 높은 정확도를 얻었다.


### 사용되는 라이브러리<br/>
***
OpenCV(4.2.0) : <https://opencv.org/releases/>


### 위조 검출 과정 및 결과<br/>
***

<img src="https://user-images.githubusercontent.com/72312559/109042353-740c9e00-7713-11eb-9175-4410ba2867d1.png" width="50%"></br>
<원본 이미지></br>
***
차례대로 DCT 후 LE변환 결과 / BAG검출 및 위조 영역 검출 / 위조 영역 가시화 / 원본에 투영이다.</br></br>
<img src="https://user-images.githubusercontent.com/72312559/109035967-7c150f80-770c-11eb-826a-71a34a2efecd.png" width="80%"></br></br></br></br></br>

***

<img src="https://user-images.githubusercontent.com/72312559/110931974-385a1100-836e-11eb-9279-0ca889abcca7.png" width="80%"></br>
***
<img src="https://user-images.githubusercontent.com/72312559/110931979-3a23d480-836e-11eb-9a70-ab814e15d21c.png" width="80%"></br>
***
<img src="https://user-images.githubusercontent.com/72312559/110931984-3abc6b00-836e-11eb-9a32-e1aa89fa13c1.png" width="40%"></br></br>
기존 연구들에 비해 L0_Norm 을 이용한 위조검출 결과가 좋은 것을 그림과 도표로 확인할 수 있다.

***
<img src="https://user-images.githubusercontent.com/72312559/110931990-3bed9800-836e-11eb-96db-1defa3b46cae.png" width="80%"></br>
<img src="https://user-images.githubusercontent.com/72312559/110931995-3d1ec500-836e-11eb-8c4e-7443235715dd.png" width="40%"></br></br>

기존 L2,L1_Norm을 이용한 위조검출에서 LE를 구할때 윈도우의 크기에 따라 성능이 정해지는 것이 아니라 이미지에 따라 성능이 정해졌다. 그러나 L0_Norm은
윈도우 크기가 64에 가까워질수록 꾸준히 좋은 성능을 안정적으로 얻어내는 것을 확인할 수 있었다. 

***


### 결론 <br/>

위변조 검출에  L_0 Norm 기반 접근방식을 사용하는 것이 모든 테스트 이미지에 대해 L_1 Norm 및 L_2 Norm 기반 접근방식 모두를 능가한다</br>

L_1 Norm은 특이치에 조금 민감하며 L_2 Norm의 경우 성능이 이미지의 복잡도에 크게 의존하므로 모든 영상에 대해 하나의 Window 크기를 정확하게 지정해 줄 수 없었으나 L_0 Norm은  Window 계수를 더 많이 사용할수록 성능이 연속적으로 증가하는 특성을 가진다.</br>

 고안한 알고리즘은 JPEG, BMP, PNG등 비압축 이미지에서 사용이 가능하다.</br>



