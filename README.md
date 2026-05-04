# Vehicle Routing & Traveling Salesman Optimization (VRP & TSP)

This project is an optimization solution developed in C for two fundamental problems in logistics and operational research: the **Traveling Salesman Problem (TSP)** and the **Vehicle Routing Problem (VRP)**. The software calculates the most efficient routes to serve a specific number of customers from a central depot using a fleet of vehicles.

## Key Features

* **Multi-Vehicle Support (VRP):** Performs route planning for multiple vehicles while considering capacity constraints.
* **Single-Vehicle Support (TSP):** If only one vehicle is available, it calculates the shortest closed-loop route visiting all nodes.
* **Smart Optimization Algorithms:** 
    * **Clarke-Wright Savings Algorithm:** Utilizes an efficient savings logic to construct initial routing solutions.
    * **2-Opt Local Search:** Implements an improvement technique that reduces total distance by reordering nodes within the generated routes.
    * **Nearest Neighbor:** Produces rapid initial solutions for quick analysis.

## File Structure

* `main.c`: The primary source code containing distance matrix generation, route merging, and optimization logic.
* `test_1car.txt`: Test data for a single-vehicle (TSP) scenario with 400 customers.
* `test_2cars.txt`: Test data for a multi-vehicle (VRP) scenario with capacity constraints.
* `test_9.txt`: A small-scale (9 customers) dataset used for algorithm validation.


# Araç Rotalama ve Gezgin Satıcı Optimizasyonu (VRP ve TSP)  

Bu proje, lojistik ve operasyonel araştırma alanındaki iki temel problem olan **Gezgin Satıcı Problemi (TSP)** ve **Araç Rotalama Problemi (VRP)** için C diliyle geliştirilmiş bir optimizasyon çözümüdür. Proje, belirli sayıdaki müşteriye depo noktasından çıkan araçlarla en kısa mesafede nasıl hizmet verileceğini hesaplar.

## Öne Çıkan Özellikler

* **Çoklu Araç Desteği (VRP):** Birden fazla aracın kapasite kısıtlarını dikkate alarak rota planlaması yapar.
* **Tek Araç Desteği (TSP):** Eğer tek bir araç varsa, tüm noktaları ziyaret eden en kısa kapalı rotayı hesaplar.
* **Akıllı Algoritmalar:**
    * **Clarke-Wright Savings Algoritması:** Başlangıç rotalarını oluşturmak için verimli bir tasarruf mantığı kullanır.
    * **2-Opt Yerel Arama:** Oluşturulan rotalardaki düğüm sıralamasını değiştirerek mesafeyi daha da kısaltan bir iyileştirme tekniği uygular.
    * **En Yakın Komşu (Nearest Neighbor):** Hızlı başlangıç çözümleri üretir.

## Dosya Yapısı

* `main.c`: Mesafe matrisi oluşturma, rota birleştirme ve optimizasyon mantığını içeren ana kaynak kod.
* `test_1car.txt`: Tek araçlı (TSP) senaryosu için 400 müşterili test verisi.
* `test_2cars.txt`: İki araçlı (VRP) senaryosu için kapasite kısıtlı test verisi.
* `test_9.txt`: Algoritmayı doğrulamak için kullanılan küçük ölçekli (9 müşteri) veri seti.
