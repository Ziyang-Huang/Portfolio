const winningCombinations = [               //the combinations of winning condition
    ["00", "01", "02"],
    ["10", "11", "12"],
    ["20", "21", "22"],
    ["00", "10", "20"],
    ["01", "11", "21"],
    ["02", "12", "22"],
    ["00", "11", "22"],
    ["02", "11", "20"]
]

class Player {
    constructor(name, chess) {
        this.name = name;
        this.chess = chess;                 //Player's chess, which should be 'O' or 'X'
        this.hand = 0;                      //0, 1 and 2 represent null, offensive and defensive
        this.stepCount = 0;
    }

    move(buttonId) {
        getSquare(board, buttonId).drop(this.chess);
        this.stepCount++;
    }

    reset() {
        this.hand = this.stepCount = 0;
    }
}

class Square {
    constructor(button) {
        this.button = button;
        this.chess = "-";
    }

    drop(chess) {
        this.button.textContent = chess;
        this.chess = chess;
    }

    isEmpty() {
        if (this.chess === "-") return true;
        else return false;
    }

    reset() {
        this.button.textContent = "";
        this.chess = "-";
    }
}

var board = [];
var human = new Player("Player", "X");
var computer = new Player("Computer", "O");

creatBoard();

function creatBoard() {
    var body = document.getElementById("body");
    var table = document.createElement("table");
    table.id = "board";
    body.appendChild(table);

    for (var i = 0; i < 3; i++) {
        var row = [];
        var tr = document.createElement("tr");
        tr.id = "row" + i;
        table.appendChild(tr);

        for (var j = 0; j < 3; j++) {
            var td = document.createElement("td");
            td.id = "square" + j;

            var button = document.createElement("button");
            button.id = "" + i + j;
            button.addEventListener("click", onClickButton, false);
            button.style = "height:200px;width:200px;background:#FFFFFF;font-size:70px";
            button.textContent = "";
            td.appendChild(button);
            tr.appendChild(td);

            var square = new Square(button);
            row.push(square);
        }
        board.push(row);
    }
}

function offensive() {
    if (!(human.stepCount + computer.stepCount)) {
        initiateGame();
        human.hand = 1;
        computer.hand = 2;
        document.getElementById("title").textContent = "TicTacToe Playing...";
    }
}

function defensive() {
    if (!(human.stepCount + computer.stepCount)) {
        initiateGame();
        human.hand = 2;
        computer.hand = 1;
        document.getElementById("title").textContent = "TicTacToe Playing...";
        turn("11", computer);
    }
}

function resetGame() {
    document.getElementById("title").textContent = "TicTacToe";
    initiateGame();
}

function initiateGame() {
    human.reset();
    computer.reset();
    inProcess = true;
    resetBoard();
}

function resetBoard() {
    for (var i = 0; i < 3; i++) {
        for (var j = 0; j < 3; j++) {
            board[i][j].reset();
        }
    }
}

function onClickButton() {
    if (human.hand === 0) {
        alert("Choose your hand!");
        return;
    }
    if (!inProcess) {
        alert('Game is over! Click "reset" button to start a new game!');
        return;
    }
    if (this.textContent == "") {
        turn(this.id, human);
        if (!checkWinner(board, human) && !isTieGame(board))
            turn("" + bestMove(), computer);
    }
}

function turn(buttonId, player) {
    player.move(buttonId);
    let winningInfo = checkWinner(board, player);
    if (winningInfo) {
        gameOver(winningInfo);
    }
    isTieGame(board);
}

function checkWinner(board, player) {
    var pos = [];
    for (var i = 0; i < 3; i++) {
        for (var j = 0; j < 3; j++) {
            if (board[i][j].chess === player.chess) {
                pos.push("" + i + j);
            }
        }
    }
    let winningInfo = null;
    for (let [index, win] of winningCombinations.entries()) {
        if (win.every(elem => pos.indexOf(elem) > -1)) {
            winningInfo = { index: index, winner: player.name };
            break;
        }
    }
    return winningInfo;
}

function isTieGame(board) {
    if (human.stepCount + computer.stepCount != 9) {
        return false;
    }
    else {
        if (!checkWinner(board, computer) && !checkWinner(board, human)) {
            inProcess = false;
            alert("Tie Game!");
            return true;
        }
    }
}

function gameOver(winningInfo) {
    inProcess = false;
    alert(winningInfo.winner + " win!");
}

function bestMove() {
    if (this.stepCount == 0) {
        if (this.hand == 1) {
            return "11";
        }
        if (this.hand == 2) {
            if (getSquareById("11").textContent == "") {
                return "11";
            }
            else {
                var choices = ["00", "02", "20", "22"]
                return choices[Math.floor(Math.random() * 4)];
            }
        }
    }
    else {
        return miniMax(board, computer).suareId;
    }

}

function miniMax(newBoard, player) {
    var availSquareId = [];
    for (var i = 0; i < 3; i++) {
        for (var j = 0; j < 3; j++) {
            if (newBoard[i][j].isEmpty() === true) {
                availSquareId.push("" + i + j);
            }
        }
    }

    if (checkWinner(newBoard, human)) {
        return { score: -10 };
    }
    else if (checkWinner(newBoard, computer)) {
        return { score: 10 };
    }
    else if (availSquareId.length === 0) {
        return { score: 0 };
    }

    var moves = [];
    for (var i = 0; i < availSquareId.length; i++) {
        var move = {};
        move.suareId = availSquareId[i];
        getSquare(newBoard, availSquareId[i]).drop(player.chess);
        if (player === computer) {
            var result = miniMax(newBoard, human);
            move.score = result.score;
        }
        else {
            var result = miniMax(newBoard, computer);
            move.score = result.score;
        }

        getSquare(newBoard, availSquareId[i]).reset();

        moves.push(move);
    }

    var bestMove;
    if (player === computer) {
        var bestScore = -10000;
        for (var i = 0; i < moves.length; i++) {
            if (moves[i].score > bestScore) {
                bestScore = moves[i].score;
                bestMove = i;
            }
        }
    }
    else {
        var bestScore = 10000;
        for (var i = 0; i < moves.length; i++) {
            if (moves[i].score < bestScore) {
                bestScore = moves[i].score;
                bestMove = i;
            }
        }
    }
    return moves[bestMove];
}

function getSquare(board, buttonId) {
    return board[parseInt(buttonId.substring(0, 1))][parseInt(buttonId.substring(1, 2))];
}