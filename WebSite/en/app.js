(function(){
    
    $(".btnPage").on("click", function(e) {
        e.preventDefault();
        var link = $(this).context.href;
        $("#displayer").attr("src", link);
    });
    
}());