# Create-Bayesian-network-in-parallel
Đề tài: song song hóa việc học cấu trúc mạng Bayesian

Tài liệu nghiên cứu: fastBN,...
Thuật toán: PC-stable
Triển khai: 
        song song cấp cạnh (Edge-level parallelism) thực hiện nhiều phép toán xác định cạnh có phải là độc lập có điều kiện không
	    song song cấp CI (CI-level parallelism) thực hiện song song hóa nhiều công việc tính toán trong từng cạnh, đặt trong 1 bể công việc động (Dynamic workpool)
Dataset: asian_10k, tic-tac-toe, survey-lungcancer, arlam_5k

lệnh chạy : g++ -fopenmp -o run Run.cpp Node.cpp Edge.cpp Skeleton.cpp method.cpp readFile.cpp      
            ./run  
