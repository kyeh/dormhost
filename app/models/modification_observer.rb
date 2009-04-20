class ModificationObserver < ActiveRecord::Observer
  
  def after_save(modification)
    
    @transaction = Transaction.find(modification.transaction_id)
    @room = Room.find(@transaction.room_id)
    @host = User.find(@room.host_id)
    @renter = User.find(@transaction.renter_id)
    ModificationMailer.deliver_notify_changes(@host, @renter, @room) if modification.recently_submit_new_modification?
    
  end
  
end
