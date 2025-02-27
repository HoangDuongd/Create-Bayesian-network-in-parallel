# Create Bayesian Network in Parallel  

## 📌 Đề tài  
**Song song hóa việc học cấu trúc mạng Bayesian**  

## 📚 Tài liệu nghiên cứu  
- fastBN  
- ...  

## 🔢 Thuật toán  
- **PC-stable**  

## 🚀 Triển khai  
1. **Song song cấp cạnh (Edge-level parallelism)**  
   - Thực hiện nhiều phép toán xác định xem một cạnh có độc lập có điều kiện hay không.  

2. **Song song cấp CI (CI-level parallelism)**  
   - Thực hiện song song hóa nhiều công việc tính toán trong từng cạnh.  
   - Đặt trong một bể công việc động (**Dynamic workpool**).  

## 📊 Dataset  
- **asian_10k**  
- **tic-tac-toe**  
- **survey-lungcancer**  
- **arlam_5k**  

## 🛠️ Cách chạy chương trình  
```sh
g++ -fopenmp -o run Run.cpp Node.cpp Edge.cpp Skeleton.cpp method.cpp readFile.cpp  
./run  
