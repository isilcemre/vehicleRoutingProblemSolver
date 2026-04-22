# Vehicle Routing & Traveling Salesman Optimization (VRP & TSP)

Bu proje, lojistik ve operasyonel araştırma alanındaki iki temel problem olan **Gezgin Satıcı Problemi (TSP)** ve **Araç Rotalama Problemi (VRP)** için C diliyle geliştirilmiş bir optimizasyon çözümüdür. Proje, belirli sayıdaki müşteriye depo noktasından çıkan araçlarla en kısa mesafede nasıl hizmet verileceğini hesaplar.

## Öne Çıkan Özellikler

* **Çoklu Araç Desteği (VRP):** Birden fazla aracın kapasite kısıtlarını dikkate alarak rota planlaması yapar.
* **Tek Araç Desteği (TSP):** Eğer tek bir araç varsa, tüm noktaları ziyaret eden en kısa kapalı rotayı hesaplar.
* **Akıllı Algoritmalar:** * **Clarke-Wright Savings Algoritması:** Başlangıç rotalarını oluşturmak için verimli bir tasarruf mantığı kullanır.
    * **2-Opt Yerel Arama:** Oluşturulan rotalardaki düğüm sıralamasını değiştirerek mesafeyi daha da kısaltan bir iyileştirme tekniği uygular.
    * **En Yakın Komşu (Nearest Neighbor):** Hızlı başlangıç çözümleri üretir.

## Dosya Yapısı

* `main.c`: Mesafe matrisi oluşturma, rota birleştirme ve optimizasyon mantığını içeren ana kaynak kod.
* `test_1car.txt`: Tek araçlı (TSP) senaryosu için 400 müşterili test verisi.
* `test_2cars.txt`: İki araçlı (VRP) senaryosu için kapasite kısıtlı test verisi.
* `test_9.txt`: Algoritmayı doğrulamak için kullanılan küçük ölçekli (9 müşteri) veri seti.
