# Finding-kcore-and-clique
我尋找k-core的做法和ppt一樣，然後使用了和維基百科上一樣的過程。首先在 輸入的時候就記錄每一個點的degree，存放在dv[]這個陣列中。接著建立一個存放陣 列的陣列D，在Di裡面存放degree為i的vertex的陣列。同時建立一個L List來放置被 刪除的點，和一個kcore的list來存放, 都先初始化為零。接著一個一個degree的跑迴 圈。從degree 0開始，把degree i的點從圖上移除，加到另一個List L裡面。這個點 的kcore就是i。把i存到kcore list中這個vertex的位置。除此之外，和這個vertex相 連的vertex，如果還沒被放到Llist的就把degree減一，放到Di-1裡面。也就是說把 跟現在所輪到的變這個點相連的邊都刪除掉，那相連的這些點的degree當然也跟著 改變。直到迴圈跑完kcore就找完了。
Clique Clique是建立在k-core找完的基礎上。在k-core的時候，就把所有k-core大於
1499的vertex存到一個list，並且用他們建立一個新的graph。接著利用ppt裡面所提 供的coloring color這個graph。在塗上顏色後，利用dfs尋找clique，不停地加入新的 vertex，如果達不到1500，就換一個vertex再繼續下一層。

