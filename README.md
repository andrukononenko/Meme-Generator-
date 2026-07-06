🎭 Meme Generator – Генератор мемов 
Создавайте мемы за секунды!
Накладывайте верхний и нижний текст на изображение, настраивайте шрифты, цвета, тени, обводку и позицию.
Пакетная обработка, поддержка популярных форматов и гибкие настройки.
Реализован на 7 языках программирования с единым интерфейсом командной строки.

🚀 Возможности
Наложение текста – верхний и нижний текст на изображении (как в классических мемах).

Настройка шрифта – выбор TTF-шрифта, размера, цвета.

Стили текста – обводка (контур), тень, жирный/курсив.

Позиционирование – автоцентрирование, отступы от краёв.

Пакетная обработка – генерация мемов для всех изображений в папке.

Поддержка форматов – PNG, JPG, BMP, WEBP (вход и выход).

Цветной вывод – информативные сообщения в терминале.

Кроссплатформенность – Windows, Linux, macOS.

📖 Использование
Синтаксис (единый для всех версий):

bash
<команда> <входное_изображение> [опции]
Опции
Опция	Описание
-t, --top <текст>	Верхний текст
-b, --bottom <текст>	Нижний текст
-o, --output <файл>	Выходной файл (по умолчанию meme.jpg)
-s, --size <N>	Размер шрифта (по умолчанию 48)
-c, --color <цвет>	Цвет текста (HEX, например #FFFFFF)
-f, --font <файл>	Путь к TTF-шрифту
--stroke <цвет>	Цвет обводки (по умолчанию чёрный)
--stroke-width <N>	Ширина обводки (по умолчанию 2)
--shadow	Добавить тень
--shadow-offset <X,Y>	Смещение тени (по умолчанию 2,2)
--padding <N>	Отступ текста от краёв (по умолчанию 20)
-b, --batch <папка>	Пакетная обработка (выходная папка)
-v, --verbose	Подробный вывод
-h, --help	Справка
Примеры
bash
# Классический мем
python meme_generator.py image.jpg -t "Верхний текст" -b "Нижний текст"

# С кастомным шрифтом и цветом
python meme_generator.py photo.png -t "Утро" -b "Вечер" -f impact.ttf -c #FF5733

# С обводкой и тенью
python meme_generator.py img.jpg -t "Hello" -b "World" --stroke #000000 --shadow

# Пакетная обработка всех JPG в папке
python meme_generator.py ./images -b ./mems -t "Мем" -b "2024"
🛠 Установка и запуск
Python
bash
pip install pillow
python meme_generator.py <image> [options]
C++
bash
sudo apt-get install libopencv-dev
g++ -std=c++17 meme_generator.cpp -o meme_generator `pkg-config --cflags --libs opencv4`
./meme_generator <image> [options]
Go
bash
go get -u github.com/fogleman/gg
go build meme_generator.go
./meme_generator <image> [options]
JavaScript (Node.js)
bash
npm install canvas
node meme_generator.js <image> [options]
C#
bash
dotnet add package SixLabors.ImageSharp
csc meme_generator.cs -r:SixLabors.ImageSharp.dll
mono meme_generator.exe <image> [options]   # или dotnet run
Ruby
bash
gem install rmagick
ruby meme_generator.rb <image> [options]
Java
bash
javac -cp .:gson.jar meme_generator.java
java -cp .:gson.jar meme_generator <image> [options]
🧠 Логика работы
Загрузка – чтение входного изображения.

Наложение текста – рисование верхнего и нижнего текста с заданными параметрами.

Стилизация – добавление обводки, тени, центрирование.

Сохранение – запись результата в выходной файл.

Пакетная обработка – повторение для всех изображений в папке.

✨ Дополнительные фичи
Автоопределение размера текста – адаптация под ширину изображения.

Многострочный текст – поддержка переноса строк (\n).

Прозрачность – поддержка PNG с альфа-каналом (опционально).

📂 Состав репозитория
Язык	Файл	Статус
Python	meme_generator.py	✅
C++	meme_generator.cpp	✅
Go	meme_generator.go	✅
JavaScript	meme_generator.js	✅
C#	meme_generator.cs	✅
Ruby	meme_generator.rb	✅
Java	meme_generator.java	✅
🤝 Вклад в проект
Приветствуются улучшения:

Добавление поддержки GIF.

Графический интерфейс.

Интеграция с веб-сервисами.

Создавайте Issues и Pull Requests.

📜 Лицензия
MIT License – свободное использование, модификация и распространение.

📂 Исходный код
Первая строка каждого файла – его имя. Скопируйте блок целиком и сохраните в соответствующий файл.
