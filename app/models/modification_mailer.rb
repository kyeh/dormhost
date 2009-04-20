class ModificationMailer < ActionMailer::Base

  def notify_changes(host, renter, room)
    setup_email(host)
    @subject    += "#{renter.username} has made changes to the room request"
    body :host => host, :renter => renter, :room => room 
    @content_type = "text/html"
  end
  
  protected

    def setup_email(user)
      @recipients  = "#{user.email}"
      @from        = "Dormhost Reservation Modification"
      @subject     = ""
      @sent_on     = Time.now
    end

end
