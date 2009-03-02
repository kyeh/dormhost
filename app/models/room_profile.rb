class RoomProfile < ActiveRecord::Base
  has_one :room
  belongs_to :level
  has_many :room_images
  has_many :room_profile_days
  
  
  attr_accessible :type, :bed_type, :max_capacity, :pets, :food, :roommates, :internet, :on_campus, :cost, :notes
  
  def is_updatable_by(user)
    user.is_admin?
  end
  
  def self.is_admin?
    self.is_admin
  end
  
  def self.smoker?
    self.smoker
  end
  
end
