class TransactionObserver < ActiveRecord::Observer
      
  def after_save(transaction)
    
    @room = Room.find(transaction.room_id)
    @host = User.find(@room.host_id)
    @renter = User.find(transaction.renter_id)
    TransactionMailer.deliver_notify_reservation(@host, @renter, @room) if transaction.recently_request_rooms?
    
  end
  
  def after_update(transaction)
    
    @room = Room.find(transaction.room_id)
    @host = User.find(@room.host_id)
    @renter = User.find(transaction.renter_id)
    TransactionMailer.deliver_notify_approval(@host, @renter, @room) if transaction.recently_approve_reservation?
    
  end
  
end
