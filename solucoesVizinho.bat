ECHO OFF

FOR /L %%A IN (1,1,30) DO (
  vizinhoBat.exe Instancias/v10.txt
)>> resultados/arquivo10.txt

FOR /L %%A IN (1,1,30) DO (
  vizinhoBat.exe Instancias/v20.txt
)>> resultados/arquivo20.txt

FOR /L %%A IN (1,1,30) DO (
  vizinhoBat.exe Instancias/v30a.txt
)>> resultados/arquivo30a.txt

FOR /L %%A IN (1,1,30) DO (
  vizinhoBat.exe Instancias/v30b.txt
)>> resultados/arquivo30b.txt

FOR /L %%A IN (1,1,30) DO (
  vizinhoBat.exe Instancias/v30c.txt
)>> resultados/arquivo30c.txt

FOR /L %%A IN (1,1,30) DO (
  vizinhoBat.exe Instancias/v50a.txt
)>> resultados/arquivo50a.txt

FOR /L %%A IN (1,1,30) DO (
  vizinhoBat.exe Instancias/v50b.txt
)>> resultados/arquivo50b.txt

FOR /L %%A IN (1,1,30) DO (
  vizinhoBat.exe Instancias/v100a.txt
)>> resultados/arquivo100a.txt

FOR /L %%A IN (1,1,30) DO (
  vizinhoBat.exe Instancias/v100b.txt
)>> resultados/arquivo100b.txt

FOR /L %%A IN (1,1,30) DO (
  vizinhoBat.exe Instancias/v250a.txt
)>> resultados/arquivo250a.txt

FOR /L %%A IN (1,1,30) DO (
  vizinhoBat.exe Instancias/v250b.txt
)>> resultados/arquivo250b.txt

FOR /L %%A IN (1,1,30) DO (
  vizinhoBat.exe Instancias/v500a.txt
)>> resultados/arquivo500a.txt

FOR /L %%A IN (1,1,30) DO (
  vizinhoBat.exe Instancias/v500b.txt
)>> resultados/arquivo500b.txt