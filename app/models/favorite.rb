class Favorite < ActiveRecord::Base
  belongs_to :room
  belongs_to :user
  
  named_scope :for_user, lambda { |user_id| { :conditions => ["user_id = ?", user_id] }}
  
  validates_presence_of :room_id, :user_id => "must be filled in"
  
end
