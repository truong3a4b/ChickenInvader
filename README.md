# ChickenInvader
## Giới thiệu
### Đề bài: Chicken Invader
### Sản phẩm: 
- Game sẽ tự động sinh kẻ địch trong màn chơi.
- Người chơi sử dụng button để di chuyển tàu bay của mình theo 4 hướng.
- Tàu bay và quái vật đều tự động nhả đạn.
- Hệ thống tính điểm cho người chơi khi tiêu diệt quái vật.
- Người chơi có tối đa 3 mạng và giảm mạng khi bị quái vật bắn trúng.
- Khi mất hết 3 mạng thì game kết thúc, người chơi xem điểm và quay về trang chủ.
## Tác giả
- Tên nhóm: Lowkey
- Thành viên:
|STT|      Họ và tên      |    MSSV    |       Công việc           |
|---|----------- ---------|------------|---------------------------|
| 1 | Nguyễn Xuân Trưởng  |  20225420  | Xây dựng logic game       |
| 2 | Nguyễn Hà Trung     |  20225417  | Xử lý kết nối ngoại vi    |
| 3 | Nguyễn Tuấn Kiệt    |  20225203  | Đánh giá và kiểm thử      |


## Môi trường hoạt động
1. Vi điều khiển trên Dev kit: STM32F429ZIT6
2. Nút bấm
3. Còi buzzer
4. Màn hình hiển thị: Màn hình LCD tích hợp trên kit.
5. Nguồn điện: Cổng USB của PC.
6. Điện trở, dây nối và testboard.
   
## Sơ đồ SCHEMATIC
|   STM32F429   |   Moudule ngoại vi    |
|---------------|-----------------------|
|      PE2      |        Nút bấm        |
|      PE3      |        Nút bấm        |
|      PE4      |        Nút bấm        |
|      PE5      |        Nút bấm        |
|      PE6      |       Còi buzzer      |

## Tích hợp hệ thống
   1. Vi điều khiển trên Dev kit: STM32F429ZIT6
   - Mô tả: Đây là bộ não trung tâm của toàn bộ hệ thống game của bạn. Nó là một chip tích hợp (System-on-Chip - SoC) bao gồm CPU, bộ nhớ (Flash và RAM), và các thiết bị ngoại vi.
   - Vai trò: 
     + Xử lý game logic: Chạy tất cả code game, từ điều khiển chuyển động của máy bay, kẻ thù, đạn, phát hiện va chạm, tính điểm, đến quản lý trạng thái game.
     + Điều khiển hiển thị: Giao tiếp với màn hình LCD để vẽ đồ họa game (hình ảnh máy bay, kẻ thù, nền, hiệu ứng, chữ).
     + Xử lý tín hiệu nhập liệu: Đọc tín hiệu từ nút nhấn để nhận lệnh điều khiển từ người chơi.
     + Điều khiển các thiết bị ngoại vi khác: Ví dụ, tạo ra âm thanh qua còi.

2. Nút bấm
   - Mô tả: Các nút nhấn vật lý
   - Vai trò: Là giao diện để người chơi tương tác và điều khiển game. Các nút nhấn dùng để điều khiển máy bay.
     
3. Còi buzzer
   - Mô tả: Còi thụ động, phát âm thanh khi có nguồn điện cung cấp
   - Vai trò: Cung cấp hiệu ứng âm thanh cho game, làm tăng trải nghiệm người chơi.
     
4. Màn hình hiển thị: Màn hình LCD tích hợp trên kit.
   - Mô tả: Là một màn hình TFT LCD được kết nối trực tiếp với vi điều khiển.
   - Vai trò: Là thiết bị đầu ra chính để hiển thị toàn bộ giao diện và nội dung game cho người chơi. Mọi thứ trong game (máy bay, kẻ thù, đạn, nền, điểm số) đều được vẽ lên đây.
     
## Đặc tả hàm 
- Khởi tạo các chần GPIO:

![Image](https://github.com/user-attachments/assets/88ec9df5-36d0-49ff-9dbf-c6c8df137ad7)

- Khởi tạo task và queue:

![Image](https://github.com/user-attachments/assets/87bbf976-b62f-4c92-ae1b-fc25b51341cd)

- Tạo task bắt sự kiên nút nhấn, gửi dữ liệu vào queue:
   Các nút bấm sẽ được cài đặt để gửi tín hiệu tới các cồng GPIO PE2, PE3, PE4, PE5. Chương trình sẽ duy trì 1 task (defaultTask) để liên tục đọc dữ liệu từ các cổng này và gửi lên hàng chờ.
![Image](https://github.com/user-attachments/assets/4b29b128-3266-4809-a6bf-8895de762f4b)

![Image](https://github.com/user-attachments/assets/4302d0eb-5037-491c-85f6-9384eb443c76)

- Khởi tạo màn hình ban đầu:
  
![image](https://github.com/user-attachments/assets/5109fc43-7ffa-4515-875b-21584b2aa1ce)

- Tạo luồng để thực thi logic game:

   ![image](https://github.com/user-attachments/assets/887ab548-bb2f-4721-9806-486f3cb38346)

  
- Lấy dữ liệu từ queue và hiển thị màn hình:
  Dữ liệu nằm trong hàng chờ sẽ được GameScreenView poll và lấy ra với mỗi lượt tick trong hàm. Sau đó GameScreenView sẽ xử lý dữ liệu và cập nhật vào đối tượng trong gameInstace (ở đây là hướng di chuyển của      ship – đối tượng mà người chơi điều khiển).
-------------------------------------------------------------------------------------------------------------------------
  // Get input
	uint8_t res = 0;
	uint32_t count = osMessageQueueGetCount(Queue1Handle);
	if(count > 0) {
			osMessageQueueGet(Queue1Handle,&res, NULL, osWaitForever);
			if(res == 'R') {

				gameInstance.ship.updateVelocityX(gameInstance.ship.VELOCITY);
				shipImage.setBitmap(touchgfx::Bitmap(BITMAP_SHIPR_ID));
				osMessageQueueReset(Queue2Handle);
			} else if(res == 'N'){
				gameInstance.ship.updateVelocityX(0);
				shipImage.setBitmap(touchgfx::Bitmap(BITMAP_SHIP_ID));
			}
	}
	uint32_t count2 = osMessageQueueGetCount(Queue2Handle);
	if(count2 > 0) {
			osMessageQueueGet(Queue2Handle,&res, NULL, osWaitForever);
			if(res == 'L') {
				gameInstance.ship.updateVelocityX(-gameInstance.ship.VELOCITY);
				shipImage.setBitmap(touchgfx::Bitmap(BITMAP_SHIPL_ID));
				osMessageQueueReset(Queue1Handle);
			} else if(res == 'N'){
				gameInstance.ship.updateVelocityX(0);
				shipImage.setBitmap(touchgfx::Bitmap(BITMAP_SHIP_ID));
			}
	}

	uint32_t count3 = osMessageQueueGetCount(Queue3Handle);
	if(count3 > 0) {
			osMessageQueueGet(Queue3Handle,&res, NULL, osWaitForever);
			if(res == 'U') {
				gameInstance.ship.updateVelocityY(gameInstance.ship.VELOCITY);
				osMessageQueueReset(Queue4Handle);
			} else if(res == 'N'){
				gameInstance.ship.updateVelocityY(0);
			}
	}

	uint32_t count4 = osMessageQueueGetCount(Queue4Handle);
	if(count4 > 0) {
			osMessageQueueGet(Queue4Handle,&res, NULL, osWaitForever);
			if(res == 'D') {
				gameInstance.ship.updateVelocityY(-gameInstance.ship.VELOCITY);
				osMessageQueueReset(Queue3Handle);
			} else if(res == 'N'){
				gameInstance.ship.updateVelocityY(0);
			}
	}
 ------------------------------------------------------------------------------------------------------

## Kết quả
https://youtube.com/shorts/of9vHE52DOw






