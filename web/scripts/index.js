function login() 
{
    console.log("Entrou Aqui !");
    window.location.href = "../pages/dashboard.html";
}

document.getElementById("login-button").addEventListener("click", login);