var xhr = new XMLHttpRequest;

xhr.open("GET", "myfile.txt");

xhr.onreadystatechange = function() {
    if (xhr.readyState == XMLHttpRequest.DONE) {
        var response = xhr.responseText;
        var SmilesData = JSON.parse(response)
        for(var i = 0; i < SmilesData.length(); i++)
        {
            console.log(SmilesData[i])
        }
    }
};
xhr.send();
