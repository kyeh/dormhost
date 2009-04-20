class TransactionMailer < ActionMailer::Base
    
  def notify_reservation(host, renter, room)
    setup_email(host)
    @subject    += "You have a new room request from #{renter.username}"
    body :host => host, :renter => renter, :room => room 
    @content_type = "text/html"
  end
  
  protected

    def setup_email(user)
      @recipients  = "#{user.email}"
      @from        = "Dormhost Reservations"
      @subject     = ""
      @sent_on     = Time.now
    end

end
