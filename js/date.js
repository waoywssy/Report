$(function() {
	$("#slider-range-min").slider({
		range: "min",
		step:0.5,
		min: 0.10,
		max: 8.00,
		slide: function(event, ui) {
			$("#time").val(ui.value);
		}
	});

	$("#type").change(
		function(){
			// No comments for 'development' and 'routineQA'
			if($(this).val() == 0 || $(this).val() == 4){
				$('#sp_comment').hide();
			}
			else{
				$('#sp_comment').show();
			}
		}
	);
	$('#sp_others').hide();
	$("#comment").change(
		function(){
			// if comment is 'others', please fill in the blank
			if($(this).val() == 6){
				$('#sp_others').show();
			}
			else{
				$('#sp_others').hide();
			}
		}
	);

	$("#time").val($("#slider-range-min").slider("value"));
	
	
	$("#datepicker").datepicker({
		dateFormat:'yy-mm-dd',
		beforeShow: function(input, datepicker) {
			setTimeout(function() {
					$(datepicker.dpDiv).css('zIndex', 100);
				}, 200);
			},
		onSelect: function(msg, inst) {
            $.ajax({
               type: "POST",
               url: "show",
               data: "date=" + msg,
               success: function(msg){
                 $("#reports-grid").html(msg);
                 deletereport();
               }
             });
        }
		});
    
    $("button").button();
    $("button#add").click(function(){
        var employee = $("#employee>option:selected").val();
        var type = $("#type>option:selected").val();
        var time = $("#time").val();
        var date = $("#datepicker").val();   
        var delivery = $("#delivery").attr("checked");
        var bot = $("#bot>option:selected").val();        
        var issue = $.trim($("#issueid").val());
        var comment = $.trim($("#comment").val());
        var others = $.trim($("#others").val());

		// No comments for 'development' and 'routineQA'
        if(type==0 || type==4){
        	comment='';
        }
        if (comment==6 && others.length < 20){
        	alert('Please type your comments, at least 20 chars!');
			return;	
        }
        if (comment!=6){
        	other = '';
        }

        if (bot == -999)
        {
			alert('Please choose a bot first!');
			return;
        }

		if (issue.length > 0 && !issue.match(/([0-9,])+/))
		{
			alert('Wrong issue type, format: "1234" or "2345, 3456..."');
			return;
		}
		if (!date.match(/^[12][0-9][0-9][0-9]\-[01][0-9]\-[0-3][0-9]$/))   
		{
			alert('Please input a date, format: "xxxx-xx-xx"');
			return;
		}
		if (!time.match(/[0-9](\.[0-9][0-9])?/))
		{
			alert('Wrong time type, format: "2" or "0.25"');
			return;
		}
		
        
        var postdata = "employee=" + employee + "&type=" + type;
        postdata += "&time=" + time + "&date=" + date;
        postdata += "&delivery=" + delivery + "&bot=" + bot + "&issue=" + issue+ "&comment=" + comment+ "&others=" + others;;
        
        $.ajax({
           type: "POST",
           url: "add",
           data: postdata,
           success: function(msg){
             //alert(msg);
             $("#reports-grid").html(msg);
             deletereport();

             $("#delivery").attr("checked", "");
             $("#issueid").val("");
           }
         });
		 
		$("#employee>option:selected").val() = employee;
    });
    $("button#view").click(function(){
        var date = $("#datepicker").val();
		
		if (!date.match(/^[12][0-9][0-9][0-9]\-[01][0-9]\-[0-3][0-9]$/))   
		{
			alert('Please input a date, format: "xxxx-xx-xx"');
			return;
		}
        var postdata = "date=" + date;
        
        $.ajax({
           type: "POST",
           url: "view",
           data: postdata,
           success: function(msg){
             //alert(msg);
             $("#reports-grid").html(msg);
             deletereport();
           }
         });
    });
    
    function deletereport()
    {
        $(".close-img").bind('click', function(){
                 
                 if(!confirm("You wanna delete this one, right?")){ return;}
                 
                 var rptid = $(this).attr("alt");
                 
                 var date = $("#datepicker").val();
                 var postdata = "reportid=" + rptid + "&date=" + date;
                 $.ajax({
                       type: "POST",
                       url: "remove",
                       data: postdata,
                       success: function(msg){
                         //alert(msg);
                         $("#reports-grid").html(msg);
                         deletereport();
                       }
                     });
             })
			 $("tr:even").css("background","#DDF0F8"); 
    }
	
	
	$("#datesform").validate({
		rules: {
			fromdate: {
				required: true,
				minlength: 2
			},
			todate: {
				required: true,
				minlength: 5
			},
		},
		messages: {
			fromdate: {
				required: "Please enter a from date",
				minlength: "Your from date must like 'yyyy-mm-dd'"
			},
			password: {
				required: "Please enter a to date",
				minlength: "Your to date must like 'yyyy-mm-dd'"
			}
		}
	});

});