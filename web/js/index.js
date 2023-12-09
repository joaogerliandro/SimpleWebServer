function enviarPedido(){
    try {
        

    const formulario = document.getElementById('pedidoForm');

    formulario.addEventListener('submit', function (e) {
        e.preventDefault();
        
       }) 

    const name = document.getElementById('cliente').value.trim().length <= 0 ? null : document.getElementById('cliente').value.trim();
    const product = document.getElementById('produto').value;
    const quantity = document.getElementById('quantidade').value.trim().length <= 0 ? null : document.getElementById('quantidade').value.trim();
    const address = document.getElementById('endereco').value.trim().length <= 0 ? null : document.getElementById('endereco').value.trim();
    const taxNumber = document.getElementById('taxnumber').value.trim().length <= 0 ? null : document.getElementById('taxnumber').value.trim();
    const valueProduct = parseInt(document.getElementById('valorProduto').innerText.replace(/[/\D]/g, ''));
    const totalValue = parseInt(document.getElementById('valorTotalPedido').innerText.replace(/[/\D]/g, ''))
    
    if(name !== null  && product !== null && quantity !== null && address !== null && valueProduct !== null && taxNumber != null){
        if(validate(name, product, quantity, valueProduct, totalValue, address, taxNumber)){
            const serviceType = {
                getProductList: 0,
                createOrder: 1,
                getOrderList: 2,
                confirmOrder: 3,
                cancelOrder: 4,
                processOrder: 5,
                updateOrderStatus: 6,
                saveLog: 7
            }
            const body = createObject(serviceType.createOrder, name, product, quantity, valueProduct, totalValue, address, taxNumber);
            let orderJSON = createJSON(serviceType.createOrder, 'endPoint', body);
            sendJSONServer(orderJSON);
        }
    }else{
        throw new Error("Não é permitido campos nulos");
    }
} catch (error) {
        showModal('error', error);
}
    
}

function validate(name, product, quantity, valueProduct, totalValue, address, taxNumber){

    if(!(name.match(/[a-z]/g) || name.match(/[A-Z]/g))){
        throw new TypeError("Tipo de caractere é inválido!");
    } 
    if(!(product === 'hamburguer' || product === 'batata' || product === 'refrigerante')){
        throw new Error("Produto inválido!");
    }
    if(!(quantity >= 0)){
        throw new Error("Quantidade informada é inválida!");
    }
    if(valueProduct <= 0){
        throw new Error("Valor do produto é inválido");
    }
    if(totalValue <= 0){
        throw new Error("Valor total do pedido é inválido");
    }
    
    if(address.length > 300 || address.length <= 0){
        throw new Error("Endereço excede o número de caracteres permitidos");
    }
    if(taxNumber.length > 11){
        throw new Error("O CPF informado é inválido");
    }
    console.log(taxNumber.length);
    if(taxNumber.match(/[a-z]/g) || taxNumber.match(/[A-Z]/g)){
        throw new Error("Insira apenas números para o CPF");
    }
    
    return true;
}


function createObject(serviceType, name, product, quantity, valueProduct, totalValue, address, taxNumber){
    console.log(serviceType)
    switch (serviceType) {
        case 0:
            return body = {
                product: product,
                valueProduct: valueProduct
            }
            
            break;
        case 1:
            return body = {
                name: name,
                product: product,
                quantity: quantity,
                valueProduct: valueProduct,
                totalValue: totalValue, 
                address: address, 
                taxNumber: taxNumber
            }
    
        default:
            throw new Error("ServiceType inválido")
    }
}

function createJSON(serviceType, remoteEndPoint, body){

    const head = {
        serviceType: serviceType,
        RemoteEndPoint: remoteEndPoint
    }
    const order = {head, body}
    console.log(order)
let jsonString = JSON.stringify(order);
return jsonString;

}




function updateProductValue(){
    let selectProduct = document.getElementById('produto');

    let priceSelected = selectProduct.options[selectProduct.selectedIndex].getAttribute('data-preco');
    let quantity = isNaN(parseInt(document.getElementById('quantidade').value.replace(/[/\D]/g, ''))) ? null : parseInt(document.getElementById('quantidade').value.replace(/[/\D]/g, ''));
    
    if(priceSelected === null){
        document.getElementById('valorProduto').innerText = ``;
        document.getElementById('valorTotalPedido').innerText = ``;
    }else{
        document.getElementById('valorProduto').innerText = `R$ ${priceSelected}`;
        if (quantity === null){
            document.getElementById('valorTotalPedido').innerText = ``;
        }else{
            document.getElementById('valorTotalPedido').innerText = `R$ ${priceSelected*quantity}`;
        }
            
    }
    
}


  function sendJSONServer(orderJSON){
      fetch('localhost', {
          method: 'POST',
          body: orderJSON
      })
      .then(response =>{
          if(!response.ok){
            
              throw new Error("Erro de envio de informações");
          }
          showModal('success', 'Pedido enviado com sucesso!')
      })
  }




function getOrdersServer(){
    try {
        const url = 'https://webhook.site/56b6ce71-7950-4f8e-aa7c-44233290c174';

        fetch(url)
        .then(response => {
    
            if(!response.ok){
                throw new Error(`Erro na requisição: ${response.status}`);
            }
    
            return response.json();
        })
        .then(data => {
            console.log(data.product);
        })  .catch(error => {
            showModal('error', error);
        });
      
        
    } catch (error) {
        showModal('error', error);
        
    }

}

function showModal(tipo, mensagem) {
    var modal = document.getElementById('modal');
    var modalContent = document.getElementById('modal-content');
  
    // Definir o tipo de modal (success ou error)
    modalContent.className = 'modal-content ' + tipo;
  
    // Exibir mensagem no modal
    document.getElementById('modal-mensagem').innerHTML = mensagem;
  
    // Exibir o modal
    modal.style.display = 'flex';
  
    // Adicionar animação
    setTimeout(function () {
      modalContent.style.transform = 'translateY(-100%)';
      modalContent.style.opacity = '0';
      
      // Adicionar um atraso antes de esconder completamente
      setTimeout(function () {
        closeModal();
      }, 500); // Ajuste conforme necessário
    }, 2000); // 2000 milissegundos (2 segundos) - ajuste conforme necessário
  }
  
  function closeModal() {
    var modal = document.getElementById('modal');
    var modalContent = document.getElementById('modal-content');
  
    // Resetar estilos de animação
    modalContent.style.transform = 'translateY(0)';
    modalContent.style.opacity = '1';
  
    // Esconder completamente
    modal.style.display = 'none';
  }
  
  
  
  
  
  
  







   
   
   
   
   
