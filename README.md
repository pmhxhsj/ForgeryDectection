# ForgeryDectection

## Digital Image Forgery Detection Using L0_Norm Local Effects

### 프로젝트 개요<br/>
***


한국연구재단에서 지원한 디지털 영상 위변조 탐지를 위한 다차원 영상특징 매트릭스 생성과 통계적 검증 모델 개발연구이다.
2020 하계학술대회를 위해 진행되었으며 대한전자공학회, 한국정보통신설비학회에 논문을 투고하였다.
디지털 이미지 위조 기술은 계속 발전하며, 시간이 갈수록 아무나 간단하게 할 수 있게 되었다. 그러나 위조를 검출하는 방법은
많이 알려지지 않았다. 이러한 문제를 해결하기 위해 L0_Norm을 이용한 복사-붙여넣기 위조 검출 알고리즘을 고안하였다.


### 프로젝트 설명<br/>
***

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

1 2 
3 4  
순서로 진행되며 1 = DCT 후 LE변환 결과 / 2 = BAG검출 및 위조 영역 검출 / 3 = 위조 영역 표시 / 4 = 기존 이미지에
투영이다.



<img src="https://user-images.githubusercontent.com/72312559/109042353-740c9e00-7713-11eb-9175-4410ba2867d1.png" width="50%"></br>
***
<img src="https://user-images.githubusercontent.com/72312559/109035967-7c150f80-770c-11eb-826a-71a34a2efecd.png" width="80%"></br></br></br></br></br>


<img src="https://user-images.githubusercontent.com/72312559/109042351-72db7100-7713-11eb-8f72-bb16d6e1feef.png" width="50%"></br>
***
<img src="https://user-images.githubusercontent.com/72312559/109035970-7cada600-770c-11eb-9dd9-06014d78a9fc.png" width="80%">
