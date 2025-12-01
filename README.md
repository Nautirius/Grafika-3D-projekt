# Symulacja ruchu ciał w polu grawitacyjnym

**Autor:** *Marcin Knapczyk*

---

## Opis projektu
GravitySim to trójwymiarowy symulator grawitacji w czasie rzeczywistym, przedstawiający interakcje pomiędzy obiektami takimi jak planety, gwiazdy czy księżyce (w tym uproszczone kolizje - większy obiekt "wchłania" mniejszy).

Projekt wykorzystuje fizykę Newtonowską, modele 3D (sfery GLU), tekstury planet/gwiazdy oraz skybox.

Użytkownik może przełączać się pomiędzy obiektami, obserwować ich ruch, wstrzymywać i zmieniać prędkość symulacji oraz ładować własne zestawy obiektów z pliku JSON (umieszczenie w odpowiednim katalogu).

---

## Nagranie działania programu:

[Demo (MP4)](video/demo.mp4)

---

## Instrukcja uruchomienia

Projekt tworzony był w środowisku MS Visual Studio. Na repozytorium dostępny jest zbudowany plik wykonywalny `x64/Debug/GravitySim.exe`. Aby uruchomić program, wystarczy uruchomić ten plik (tekstury i przykładowy plik z listą obiektów `example_sims/scene.json` znajdują się w odpowiednich katalogach).

---

## Sterowanie 

* Kamera: Orbit Controls za pomocą myszy (LPM + przesunięcie, Scroll)
* Przełączenie Obiektu: Tab
* Wstrzymanie/wznowienie symulacji: Spacja
* Pokazanie/ukrycie siatki: G
* Przyspieszenie/spowolnienie symulacji: M/N
* Przywrócenie standardowej prędkości symulacji: B
* Wyjście z programu: Q


---

## Wykorzystane biblioteki (NuGet):

* glew.v140
* glm
* nlohman.json
* sfml_graphics
* sfml_system
* sfml_window
* imgui

---

## Model oświetlenia:

W projekcie wykorzystany jest model oświetlenia Phonga.

Planety oświetlone są światłem gwiazdy (dla uzyskania efektu "świecenia" użyto blendingu).

---

## Edycja listy obiektów:

Program ładuje listę obiektów biorących udział w symulacji z pliku `example_sims/scene.json` (relatywnie względem pliku .exe).
 
Przykładowa lista obiektów:

```json
[
  {
    "name": "Sun",
    "pos": [0,0,-350],
    "vel": [0,0,0],
    "mass": 1.989e25,
    "density": 5515,
    "color": [1,1,0.9,1],
    "rotSpeed": -0.1,
    "glow": true,
    "texture": "textures/sun_texture_1.jpg"
  },
  {
    "name": "Earth",
    "pos": [5000,650,-350],
    "vel": [0,0,-300],
    "mass": 5.97219e22,
    "density": 5515,
    "rotSpeed": -2.0,
    "texture": "textures/earth_texture_1.jpg"
  },
  {
    "name": "Moon",
    "pos": [4850,650,-200],
    "vel": [-100,0,-400],
    "mass": 5.97219e21,
    "density": 5515,
    "rotSpeed": 1.0,
    "texture": "textures/moon_texture_1.jpg"
  },
  {
    "name": "Jupiter",
    "pos": [-5000,650,-350],
    "vel": [0,100,300],
    "mass": 5.97219e23,
    "density": 5515,
    "rotSpeed": -0.5,
    "texture": "textures/jupiter_texture_1.jpg"
  }
]
```
