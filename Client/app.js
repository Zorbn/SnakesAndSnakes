const bgCanvas = document.createElement("canvas");
const bgCtx = bgCanvas.getContext('2d');
const fgCanvas = document.getElementById("gameCanvas");
const fgCtx = fgCanvas.getContext('2d');

const mapWidth = 40;
const mapHeight = 30;
const tileSize = 16;

const bgColor = '#cc44ee';
const grassColors = ['#99ee44', '#96dc43'];
const appleColor = '#ff0000';
const localSnakeHeadColor = '#ffffff';

const drawSnake = (snake, localId) => {
	let i = 0;
	for (let seg of snake.segments) {
		let color = i == 0 && snake.id == localId ? localSnakeHeadColor : snake.color;
		fgCtx.fillStyle = color;
		fgCtx.fillRect(seg.x * tileSize, seg.y * tileSize, tileSize, tileSize);
		++i;
	}
};

const drawApple = (apple) => {
	fgCtx.fillStyle = appleColor;
	fgCtx.fillRect(apple.x * tileSize, apple.y * tileSize, tileSize, tileSize);
};

const drawBg = () => {
	fgCtx.drawImage(bgCanvas, 0, 0);
};

const draw = (snakes, apples, localId) => {
	drawBg();

	for (let apple of apples) {
		drawApple(apple);
	}

	for (let snake of snakes) {
		drawSnake(snake, localId);
	}
};

let keysPressed = new Set();

window.addEventListener('keydown', (event) => {
	keysPressed.add(event.key);
});

window.addEventListener('keyup', (event) => {
	keysPressed.delete(event.key);
});

bgCanvas.width = mapWidth * tileSize;
bgCanvas.height = mapHeight * tileSize;
fgCanvas.width = mapWidth * tileSize;
fgCanvas.height = mapHeight * tileSize;

bgCtx.fillStyle = '#ee66ff';
bgCtx.fillRect(0, 0, bgCanvas.width, bgCanvas.height);

for (let x = 0; x < mapWidth; ++x) {
	for (let y = 0; y < mapHeight; ++y) {
		bgCtx.fillStyle = grassColors[(x + y) % 2];
		bgCtx.fillRect(x * tileSize, y * tileSize, tileSize, tileSize);
	}
}

drawBg();

const playButton = document.getElementById('playButton');
const lobbyNameInput = document.getElementById('lobbyNameInput');
const ui = document.getElementById('ui');

playButton.addEventListener('click', () => {
	ui.style.display = "none";

	const lobbyName = lobbyNameInput.value;

	socket = new WebSocket(`ws://localhost:3000/chat?room_name="${lobbyName}"`);

	console.log('Trying to connect...');

	let dirX = 1, dirY = 0;
	let localId = 0;

	const update = () => {
		let newDirX = 0, newDirY = 0;

		if (keysPressed.has("ArrowLeft")) newDirX -= 1;
		if (keysPressed.has("ArrowRight")) newDirX += 1;
		if (keysPressed.has("ArrowUp")) newDirY -= 1;
		if (keysPressed.has("ArrowDown")) newDirY += 1;

		let isDirNonZero = newDirX != 0 || newDirY != 0;
		let isNewDirDifferent = newDirX != dirX || newDirY != dirY;
		let isNewDir1D = newDirX == 0 || newDirY == 0;
		let isNewDirNotReverse = newDirX != -dirX || newDirY != -dirY;

		if (isDirNonZero && isNewDir1D && isNewDirNotReverse && isNewDirDifferent) {
			dirX = newDirX;
			dirY = newDirY;

			let msg = {
				type: "changeDirection",
				dirX,
				dirY
			};

			socket.send(JSON.stringify(msg));
		}

		window.requestAnimationFrame(update);
	};

	socket.addEventListener('open', (event) => {
		console.log('Connected!');
		window.requestAnimationFrame(update);
	});

	socket.addEventListener('message', (event) => {
		const state = JSON.parse(event.data);

		if (state.type == "state") {
			draw(state.snakes, state.apples, localId);
		} else if (state.type == "init") {
			localId = state.localId;
        }
	});
});