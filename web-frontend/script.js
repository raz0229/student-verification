let selected, roll;

let year = document.querySelector(".cp-text #year");
year.textContent = new Date().getFullYear();

document.querySelector('ul').addEventListener('click', (x) => {
    selected = x.target.textContent
    document.querySelector('span').innerText = selected;
    document.querySelector('#touch').checked = false
})

document.querySelector('.submit').addEventListener('click', (e)=> {
    e.preventDefault();
    roll = document.querySelector('#roll').value
    if (!selected)
        document.querySelector('nav').style.borderColor = '#b00';
    else if (!roll) {
        document.querySelector('#roll').style.borderWidth = '1px';
        document.querySelector('#roll').style.borderColor = '#b00';
    }
    else {
        console.log(selected, roll)
        document.querySelector('.error').style.display = 'block'
    }
    
})