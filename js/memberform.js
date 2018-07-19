(function($, Drupal) {
  Drupal.ajax.prototype.commands.memberform_ga = function(ajax, response, status) {
    if(window.ga && ga.create) {
      ga('set', { page: response.path, title: response.title });
      ga('send', 'pageview');
    }
  };
  Drupal.behaviors.memberformModule = {
    attach: function (context, settings) {
      // Code to be run on page load, and
      // on ajax load added here
      if ( $( ".messages.error" ).first().length ) {
        $('html, body').animate({
          scrollTop: ($('.messages.error').first().parent().offset().top)
        },500);
      }
      else {
        $('html, body').animate({
          scrollTop: ($('h3').first().parent().offset().top)
        },500);
      }
    }
  };
})(jQuery, Drupal);
