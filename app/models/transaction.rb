class Transaction < ActiveRecord::Base
  #has_many :modifications  #This relationship messes up creating a new room review for some reason
  belongs_to :renter
  belongs_to :room
  #has_many :room_reviews   #This relationship messes up creating a new room review for some reason

end
