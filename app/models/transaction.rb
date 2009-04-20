class Transaction < ActiveRecord::Base
  #has_many :modifications  #This relationship messes up creating a new room review for some reason
  belongs_to :renter
  belongs_to :room
  #has_many :room_reviews   #This relationship messes up creating a new room review for some reason

  def request_rooms
    @requested_rooms = true
  end
  
   def submit_new_modification
    @new_modification = true
  end
  
  #used in Transaction obeserver
  def recently_request_rooms?
    @requested_rooms
  end
  
  def recently_submit_new_modification?
    @new_modification
  end
  
end
